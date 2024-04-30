#include "token.h"
#include <QRegularExpression>

using namespace avs8086::token;

Token::Token()
    : Token(ILLEGAL, "", 0, 0)
{ }

Token::Token(Type type, const QString& literal, int row, int column)
    : m_type(type), m_literal(literal), m_row(row), m_column(column)
{ }

#if 0
Token::Token(const Token& other)
    : Token(other.m_type, other.m_literal, other.m_row, other.m_column)
{ }

Token& Token::operator=(const Token& other)
{
    if (this != &other)
    {
        this->m_type = other.m_type;
        this->m_literal = other.m_literal;
        this->m_row = other.m_row;
        this->m_column = other.m_column;
    }
    return *this;
}
#endif

/* ========================================================================== */

void Token::resetType(Type type)
{
    m_type = type;
}

bool Token::is(Type type) const
{
    return m_type == type;
}

Token::Type Token::type() const
{
    return m_type;
}

QString Token::literal() const
{
    return m_literal;
}

int Token::length() const
{
    return m_literal.length();
}

int Token::row() const
{
    return m_row;
}

int Token::column() const
{
    return m_column;
}

int Token::endColumn() const
{
    return m_column + m_literal.length();
}

QString Token::typeName() const
{
    return typeName(m_type);
}

/* ========================================================================== */

QString Token::typeName(Type type)
{
#if 0
    auto find = [type](const QList<const QMap<Token::Type, QString>*>& map) {
        for (const auto& m : map)
        {
            auto it = m->find(type);
            if (it != m->end())
                return it.value();
        }
        return QString("Illegal");
    };
    return find({&sm_typeNames, &sm_mnemonics_A, &sm_symbols_A});
#endif
    auto it = sm_typeNames.find(type);
    if (it != sm_typeNames.end())
        return it.value();
    return sm_illegalName;
}

Token::Type Token::type(const QChar& literal)
{
    auto it = sm_singleSymbols.find(literal);
    if (it != sm_singleSymbols.end())
        return it.value();
    return ILLEGAL;
}

Token::Type Token::type(const QChar& l1, const QChar& l2)
{
    auto it = sm_doubleSymbols.find(QString(l1) + l2);
    if (it != sm_doubleSymbols.end())
        return it.value();
    return ILLEGAL;
}

Token::Type Token::type(const QString& literal)
{
    if (literal.isEmpty())
    {
        return TOKEN_EOF;
    }
    auto up = literal.toUpper();
    auto head = up.at(0);
    auto tail = up.at(up.length() - 1);

    if (head == ';')
    {
        return ANNOTATION;
    }
    else if ((head == '\'' || head == '\"') && head == tail && up.length() > 1)
    {
        return STRING;
    }
    else if (head.isDigit())
    { // 判断数字
        int num = textToInt(up);
        if (num >= 0)
        {
            return INTEGER;
        }
        else if (num == -2)
        {
            return ILLEGAL_INTEGER;
        }
        else if (textIsFloat(up))
        {
            return FLOAT;
        }
    }
    else
    {
        // 判断运算符
        if (up.length() == 1)
        {
            auto t = type(up.at(0));
            if (t != ILLEGAL)
                return t;
        }
        else if (up.length() == 2)
        {
            auto t = type(up.at(0), up.at(1));
            if (t != ILLEGAL)
                return t;
        }
#if 0
        for (auto it = sm_setTypes.begin(); it != sm_setTypes.end(); it++)
        {
            if (it.key()->contains(up))
                return it.value();
        }
#endif
        // 判断助记符
        auto it = sm_mnemonics.find(up);
        if (it != sm_mnemonics.end())
        {
            return it.value();
        }
    }
    return ILLEGAL;
}

int Token::textToInt(const QString& numStr)
{
    static const QRegularExpression regex(
        "^(0X|[0-9A-F])([0-9A-F]*?)([HDOB])?$"
    );

    QRegularExpressionMatch match = regex.match(numStr.toUpper());

    if (match.hasMatch())
    {
        QString hex = match.captured(1);
        QString number = match.captured(2);
        QString suffix = match.captured(3);
        bool ok;
        int num, suf = 10;

        if (hex.length() == 2)
        {
            number.append(suffix);
            suf = 16;
        }
        else
        {
            number.push_front(hex);
            if (!suffix.isEmpty() && suffix != "D")
            {
                if (suffix == "H")
                    suf = 16;
                else if (suffix == "O")
                    suf = 8;
                else if (suffix == "B")
                    suf = 2;
            }
        }
        // num 为 uint, 负数用来表示失败状态
        num = qAbs(number.toUInt(&ok, suf));
        if (ok)
            return num;
        return -2;
    }
    return -1;
}

bool Token::textIsFloat(const QString& numStr)
{
    bool ok;
    numStr.toDouble(&ok);
    return ok;
}

/* ========================================================================== */


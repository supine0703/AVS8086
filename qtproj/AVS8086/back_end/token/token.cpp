#include "token.h"
#include <QRegularExpression>

using namespace avs8086::token;

/* ========================================================================== */

QString Token::content() const
{
    auto it = sm_contents.find(m_type);
    if (it != sm_contents.end())
        return it.value().arg(m_literal);
    return typeName();
}

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

Token::Type Token::type(const QChar& literal1, const QChar& literal2)
{
    auto it = sm_doubleSymbols.find(QString(literal1) + literal2);
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
    else if (literal == "\n")
    {
        return TOKEN_EOL;
    }

    auto up = literal.toUpper();
    auto& head = up.front();
    auto& tail = up.back();

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
        auto num = textIsInteger(up);
        if (num == 1)
        {
            return INTEGER;
        }
        else if (num == -1)
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

/* ========================================================================== */

bool Token::textIsFloat(const QString& numStr)
{
    bool ok;
    numStr.toDouble(&ok);
    return ok;
}

int Token::textIsInteger(const QString& numStr, size_t* num)
{
    static const QRegularExpression regex(
        "^(0X|[0-9A-F])([0-9A-F]*?)([HDOB])?$"
    );
    auto match = regex.match(numStr.toUpper());

    if (!match.hasMatch())
        return 0;

    auto hex = match.captured(1);
    auto number = match.captured(2);
    auto suffix = match.captured(3);
    int suf = 10;

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
    bool ok;
    size_t n = number.toULongLong(&ok, suf);
    if (!ok)
        return -1;
    if (num)
        *num = n;
    return 1;
}

/* ========================================================================== */

#if 0
Token::Token()
    : Token(ILLEGAL, "", 0, 0)
{ }

Token::Token(Type type, const QString& literal, int row, int column)
    : m_type(type), m_literal(literal), m_row(row), m_column(column)
{ }


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

/* ========================================================================== */

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

qsizetype Token::length() const
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
#endif

/* ========================================================================== */

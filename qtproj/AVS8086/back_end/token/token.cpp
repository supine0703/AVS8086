#include "token.h"
#include <QRegularExpression>

using namespace avs8086::token;

Token::Token()
    : Token(ILLEGAL, "", 0, 0)
{ }

Token::Token(Type type, const QString& literal, int row, int column)
    : m_type(type), m_literal(literal), m_row(row), m_column(column)
{ }

Token::Token(const Token& other)
    : m_type(other.m_type)
    , m_literal(other.m_literal)
    , m_row(other.m_row)
    , m_column(other.m_column)
{ }

Token::~Token()
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

QString Token::typeName() const
{
    return typeName(m_type);
}

/* ========================================================================== */

QString Token::typeName(Type type)
{
    auto find = [type](const QList<const QMap<Token::Type, QString>*>& map) {
        for (const auto& m : map)
        {
            auto it = m->find(type);
            if (it != m->end())
                return it.value();
        }
        return QString("");
    };

    return find({&sm_typeNames, &sm_mnemonics_A, &sm_symbols_A});
}

Token::Type Token::tokenType(const QChar& literal)
{
    auto it = sm_symbols_B.find(literal);
    if (it != sm_symbols_B.end())
        return it.value();
    return ILLEGAL;
}

Token::Type Token::tokenType(const QChar& l1, const QChar& l2)
{
    QString s(l1);
    auto it = sm_symbols_C.find(s.append(l2));
    if (it != sm_symbols_C.end())
        return it.value();
    return ILLEGAL;
}

Token::Type Token::tokenType(const QString& literal)
{
    if (literal.isEmpty())
    {
        return TOKEN_EOF;
    }
    auto up(literal.toUpper());
    auto& head = up.at(0);
    auto& tail = up.at(up.length() - 1);

    if (head == ';')
    {
        return ANNOTATION;
    }
    else if ((head == '\'' || head == '\"') && head == tail && up.length() > 1)
    {
        return STRING;
    }
    else if (sm_registers.contains(up))
    {
        return REGISTER;
    }
    else if (sm_makeNames.contains(up))
    {
        return MAKE_X;
    }
    else if (sm_loadNames.contains(up))
    {
        return LOAD_X;
    }
    else
    {
        // 判断数字
        int num = textToInt(up);
        if (num > 0)
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

        // 判断助记符
        auto it = sm_mnemonics_B.find(up);
        if (it != sm_mnemonics_B.end())
        {
            return it.value();
        }
        else
        {
            if (up.length() == 1)
            {
                return tokenType(up.at(0));
            }
            else if (up.length() == 2)
            {
                return tokenType(up.at(0), up.at(1));
            }
        }
    }
    return ILLEGAL;
}

int Token::textToInt(const QString& numStr)
{
    static const QRegularExpression regex("^(0X|[0-9])([0-9A-F]*?)([HDOB])?$");

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
        num = number.toInt(&ok, suf);
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

int Token::findRegisters(const QString& reg)
{
    return sm_registers.indexOf(reg);
}

/* ========================================================================== */

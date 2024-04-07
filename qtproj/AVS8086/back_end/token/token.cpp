#include "token.h"
#include <QRegularExpression>

using namespace avs8086::token;

Token::Token()
    : Token(TOKEN_ILLEGAL, "", 0, 0)
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

bool Token::is(Type type) const
{
    return m_type == type;
}

/* ========================================================================== */

Token::Type Token::type() const
{
    return m_type;
}

QString Token::literal() const
{
    return m_literal;
}

int Token::row() const
{
    return m_row;
}

int Token::column() const
{
    return m_column;
}

void Token::resetType(Type type)
{
    m_type = type;
}

QString Token::typeName() const
{
    return tokenTypeName(m_type);
}

/* ========================================================================== */

QString Token::tokenTypeName(Type type)
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
    return TOKEN_ILLEGAL;
}

Token::Type Token::tokenType(const QChar& l1, const QChar& l2)
{
    QString s(l1);
    auto it = sm_symbols_C.find(s.append(l2));
    if (it != sm_symbols_C.end())
        return it.value();
    return TOKEN_ILLEGAL;
}

Token::Type Token::tokenType(const QString& literal)
{
    if (literal.isEmpty())
    {
        return TOKEN_EOF;
    }
    auto up(literal.toUpper());
    auto head = up.at(0);
    auto tail = up.at(up.length() - 1);

    if (head == ';')
    {
        return TOKEN_ANNOTATION;
    }
    else if ((head == tail) && (head == '\'' || head == '\"') && up.length() > 1)
    {
        return TOKEN_STRING;
    }
    else if (textToInt(up) != -1)
    {
        return TOKEN_INTEGER;
    }
    else if (textIsFloat(up))
    {
        return TOKEN_FLOAT;
    }
    else if (sm_registers.contains(up))
    {
        return TOKEN_REGISTER;
    }
    else if (sm_makeNames.contains(up))
    {
        return TOKEN_MAKE_;
    }
    else if (sm_loadNames.contains(up))
    {
        return TOKEN_LOAD_;
    }
    else
    {
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
            else
                return TOKEN_ILLEGAL;
        }
    }
}

int Token::textToInt(const QString& numStr)
{
    static const QRegularExpression regex(
        "^(0X|[0-9])([0-9A-F]*?)([HDOB])?$",
        QRegularExpression::CaseInsensitiveOption
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
        num = number.toInt(&ok, suf);
        if (ok)
            return sm_lastTextToInt = num;
        return sm_lastTextToInt = -2;
    }
    return sm_lastTextToInt = -1;
}

int Token::lastTextToInt()
{
    return sm_lastTextToInt;
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

int Token::sm_lastTextToInt = -1;

const QStringList Token::sm_makeNames = {
    "MAKE_BIN", "MAKE_COM", "MAKE_EXE",
};

const QStringList Token::sm_loadNames = {
    "LOAD_SEGMENT", "LOAD_OFFSET",
};


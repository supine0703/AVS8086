#include "token.h"

#include <QRegularExpression>

using namespace lzl;
using namespace lzl::a1::token;

/* ========================================================================== */

QString Token::content() const
{
    auto it = s_contents.find(m_type);
    if (it != s_contents.end())
    {
        return it.value().arg(m_literal);
    }
    return typeName();
}

size_t Token::toInt(bool* ok) const
{
    size_t n = 0;
    if (ok != nullptr)
    {
        *ok = (textIsInteger(m_literal, &n) == 1);
    }
    else
    {
        textIsInteger(m_literal, &n);
    }
    return n;
}

QString Token::typeName(Type type)
{
    auto it = s_typesName.find(type);
    if (it != s_typesName.end())
    {
        return it.value();
    }
    return s_illegalName;
}

Token::Type Token::type(const QChar& literal)
{
    auto it = s_singleSymbols.find(literal);
    if (it != s_singleSymbols.end())
    {
        return it.value();
    }
    return ILLEGAL;
}

Token::Type Token::type(const QChar& literal1, const QChar& literal2)
{
    auto it = s_doubleSymbols.find(QString(literal1) + literal2);
    if (it != s_doubleSymbols.end())
    {
        return it.value();
    }
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
            {
                return t;
            }
        }
        else if (up.length() == 2)
        {
            auto t = type(up.at(0), up.at(1));
            if (t != ILLEGAL)
            {
                return t;
            }
        }
#if 0
        for (auto it = s_setTypes.begin(); it != s_setTypes.end(); it++)
        {
            if (it.key()->contains(up))
                return it.value();
        }
#endif
        // 判断助记符
        auto it = s_mnemonics.find(up);
        if (it != s_mnemonics.end())
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
    static const QRegularExpression regex("^(0X|[0-9A-F])([0-9A-F]*?)([HDOB])?$");
    auto match = regex.match(numStr.toUpper());

    if (!match.hasMatch())
    {
        return 0;
    }

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
            {
                suf = 16;
            }
            else if (suffix == "O")
            {
                suf = 8;
            }
            else if (suffix == "B")
            {
                suf = 2;
            }
        }
    }
    bool ok;
    size_t n = number.toULongLong(&ok, suf);
    if (!ok)
    {
        return -1;
    }
    if (num)
    {
        *num = n;
    }
    return 1;
}

/* ========================================================================== */

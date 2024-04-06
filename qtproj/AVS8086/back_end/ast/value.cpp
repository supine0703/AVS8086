#include "value.h"
#include "qdebug.h"

using namespace avs8086::ast;

Value::Value(int v)
{
    int bytes = 4;
    do
    {
        m_value.append(v & 0xff);
        v >>= 8;
    } while (v && --bytes);
}

Value::Value(const char* v)
    : m_value(v)
{
    least(m_value);
}

Value::Value(const std::string& v)
    : m_value(v.c_str())
{
    least(m_value);
}

Value::Value(const QString& v)
    : m_value(v.toUtf8())
{
    least(m_value);
}

Value::Value(const QByteArray& v)
    : m_value(v)
{
    least(m_value);
}

Value::Value(const Value& other)
    : m_value(other.m_value)
{
    least(m_value);
}

Value::~Value()
{ }

Value& Value::operator=(const Value& other)
{
    this->m_value = other.m_value;
    least(this->m_value);
    return *this;
}

int Value::toInt() const
{
    // int value = 0;
    // for (int i = m_value.length() - 1; i >= 0; --i)
    // {
    //     value |= m_value.at(i);
    //     value <<= 8;
    // }
    // return value;
    return m_value.toHex().toUInt(nullptr, 16);
}

QString Value::toHex(char c) const
{
    return m_value.toHex(c);
}

QString Value::toString() const
{
    return m_value.toStdString().c_str();
}

QByteArray Value::value() const
{
    return m_value;
}

bool Value::isExtend() const
{
    return m_extend;
}

void Value::setExtend(bool yes)
{
    m_extend = yes;
}

/* ========================================================================== */

Value Value::operator~() const
{
    auto value = m_value;
    for (auto& v : value)
        v = ~v;
    least(value);
    return value;
}

Value Value::operator+() const
{
    return m_value;
}

Value Value::operator-() const
{
    auto value = m_value;
    for (auto& v : value)
        v = 0 - v;
    least(value);
    return value;
}

/* ========================================================================== */

Value Value::operator+(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    QByteArray nb(len + 1, 0);

    for (int i = 0; i < len; i++)
    {
        qint16 sum = static_cast<quint8>(nb1.at(i));
        sum += static_cast<quint8>(nb2.at(i));
        nb[i] += sum & 0xff;
        nb[i + 1] += (sum >> 8) & 0xff;
    }

    if (!m_extend)
        nb.removeLast();

    least(nb);
    return nb;
}

Value Value::operator-(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    QByteArray nb(len + 1, 0);

    for (int i = 0; i < len; i++)
    {
        qint16 sum = static_cast<quint8>(nb1.at(i));
        sum -= static_cast<quint8>(nb2.at(i));
        nb[i] += sum & 0xff;
        nb[i + 1] += (sum >> 8) & 0xff;
    }

    if (!m_extend)
        nb.removeLast();

    least(nb);
    return nb;
}

Value Value::operator*(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    QByteArray nb(len << 1, 0);

    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            qint16 sum = static_cast<quint8>(nb1.at(i));
            sum *= static_cast<quint8>(nb2.at(j));
            nb[i + j] += sum & 0xff;
            nb[i + j + 1] += (sum >> 8) & 0xff;
        }
    }

    if (!m_extend)
        return nb.left(len);

    least(nb);
    return nb;
}

Value Value::operator/(const Value& other) const
{
    if (other == 0)
        return 0;
    if (*this == 0)
    {
        QByteArray b;
        b.resize(1, 0);
        return b;
    }
    Value nv1 = *this;
    int len = nv1.m_value.length();

    Value nv;

    while ((nv1 -= other).m_value.length() == len)
        nv += 1;

    if (!m_extend && nv.m_value.length() > this->m_value.length())
        nv.m_value.resize(this->m_value.length());

    return nv.m_value;
}

Value Value::operator%(const Value& other) const
{
    if (other == 0)
        return 0;
    if (*this == 0)
    {
        QByteArray b;
        b.resize(1, 0);
        return b;
    }
    Value nv1 = *this;
    int len = nv1.m_value.length();

    Value nv;

    while ((nv1 -= other).m_value.length() == len)
        nv += 1;

    nv1.m_value.removeLast();
    nv1.setExtend(false);

    return nv1 + other;
}

Value Value::operator&(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    QByteArray nb(len + 1, 0);

    for (int i = 0; i < len; i++)
    {
        nb[i] = nb1.at(i) & nb2.at(i);
    }

    least(nb);
    return nb;
}

Value Value::operator^(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    QByteArray nb(len + 1, 0);

    for (int i = 0; i < len; i++)
    {
        nb[i] = nb1.at(i) ^ nb2.at(i);
    }

    least(nb);
    return nb;
}

Value Value::operator|(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    QByteArray nb(len + 1, 0);

    for (int i = 0; i < len; i++)
    {
        nb[i] = nb1.at(i) | nb2.at(i);
    }

    least(nb);
    return nb;
}

Value Value::operator<<(const Value& other) const
{
    Value count = other;
    Value nv = *this;
    while (count != 0)
    {
        count -= 1;
        nv *= 2;
    }
    return nv.m_value;
}

Value Value::operator>>(const Value& other) const
{
    Value count = other;
    Value nv = *this;
    while (count != 0)
    {
        count -= 1;
        nv /= 2;
    }
    return nv.m_value;
}

/* ========================================================================== */

Value& Value::operator+=(const Value& other)
{
    return *this = *this + other;
}

Value& Value::operator-=(const Value& other)
{
    return *this = *this - other;
}

Value& Value::operator*=(const Value& other)
{
    return *this = *this * other;
}

Value& Value::operator/=(const Value& other)
{
    return *this = *this / other;
}

Value& Value::operator%=(const Value& other)
{
    return *this = *this % other;
}

Value& Value::operator&=(const Value& other)
{
    return *this = *this & other;
}

Value& Value::operator^=(const Value& other)
{
    return *this = *this ^ other;
}

Value& Value::operator|=(const Value& other)
{
    return *this = *this | other;
}

Value& Value::operator<<=(const Value& other)
{
    return *this = *this << other;
}

Value& Value::operator >>=(const Value& other)
{
    return *this = *this >> other;
}

/* ========================================================================== */

bool Value::operator>(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    bool flg = false;

    for (int i = len - 1; i >= 0; i--)
    {
        if (nb1.at(i) != nb2.at(i))
        {
            flg = nb1.at(i) > nb2.at(i);
            break;
        }
    }

    return flg;
}

bool Value::operator>=(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    bool flg = true;

    for (int i = len - 1; i >= 0; i--)
    {
        if (nb1.at(i) != nb2.at(i))
        {
            flg = nb1.at(i) > nb2.at(i);
            break;
        }
    }

    return flg;
}

bool Value::operator<(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    bool flg = false;

    for (int i = len - 1; i >= 0; i--)
    {
        if (nb1.at(i) != nb2.at(i))
        {
            flg = nb1.at(i) < nb2.at(i);
            break;
        }
    }

    return flg;
}

bool Value::operator<=(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    bool flg = true;

    for (int i = len - 1; i >= 0; i--)
    {
        if (nb1.at(i) != nb2.at(i))
        {
            flg = nb1.at(i) < nb2.at(i);
            break;
        }
    }

    return flg;
}

bool Value::operator==(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    bool flg = true;

    for (int i = len - 1; i >= 0; i--)
    {
        if (nb1.at(i) != nb2.at(i))
        {
            flg = false;
            break;
        }
    }

    return flg;
}

bool Value::operator!=(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    bool flg = false;

    for (int i = len - 1; i >= 0; i--)
    {
        if (nb1.at(i) != nb2.at(i))
        {
            flg = true;
            break;
        }
    }

    return flg;
}

/* ========================================================================== */

int Value::init_nb(
    QByteArray& nb1, QByteArray& nb2,
    const QByteArray& b1, const QByteArray& b2)
{
    nb1 = b1;
    nb2 = b2;
    bool nbf = b1.length() > b2.length();
    int len;
    if (nbf)
    {
        len = b1.length();
        if (nb2.isEmpty())
            nb2.resize(len, 0);
        else
            nb2.resize(len, nb2.at(nb2.length() - 1) < 0 ? 0xff : 0);
    }
    else
    {
        len = b2.length();
        if (nb1.isEmpty())
            nb1.resize(len, 0);
        nb1.resize(len, nb1.at(nb1.length() - 1) < 0 ? 0xff : 0);
    }
    return len;
}

void Value::least(QByteArray& b)
{
    while (!b.isEmpty() && b.at(b.length() - 1) == 0)
        b.removeLast();

    if (b.isEmpty())
        b.resize(1, 0);
}

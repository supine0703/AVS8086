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
    least(m_value);
}

Value::Value(quint64 v)
{
    int bytes = 8;
    do
    {
        m_value.append(v & 0xff);
        v >>= 8;
    } while (v && --bytes);
    least(m_value);
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

// bool Value::isExtend() const
// {
//     return m_extend;
// }

// void Value::setExtend(bool yes)
// {
//     m_extend = yes;
// }

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
        sum += static_cast<quint8>(nb2.at(i)) + nb[i];
        nb[i] = sum & 0xff;
        nb[i + 1] += (sum >> 8) & 0xff;
    }

    // if (!m_extend)
    //     nb.removeLast();

    least(nb);
    return nb;
}

Value Value::operator-(const Value& other) const
{
    QByteArray nb1, nb2;

    int len = init_nb(nb1, nb2, this->m_value, (-other).m_value);

    QByteArray nb(len + 1, 0);

    for (int i = 0; i < len; i++)
    {
        qint16 sum = static_cast<quint8>(nb1.at(i));
        sum += static_cast<quint8>(nb2.at(i)) + nb[i];
        nb[i] = sum & 0xff;
        nb[i + 1] += (sum >> 8) & 0xff;
    }

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

    // if (!m_extend)
    //     return nb.left(len);

    least(nb);
    return nb;
}

Value Value::operator/(const Value& other) const
{
    // if (this->m_value.length() <= 8 && other.m_value.length() <= 8)
    // {
    //     quint64 v1 = 0, v2 = 0;
    //     for (const auto& va : this->m_value)
    //     {
    //         v1 += va;
    //         v1 <<= 8;
    //     }
    //     for (const auto& va : other.m_value)
    //     {
    //         v2 += va;
    //         v2 <<= 8;
    //     }
    //     int len = qMax(this->m_value.length(), other.m_value.length());

    //     return (v1 / v2);
    // }


    if (other == 0)
        return 0;
    if (*this == 0)
    {
        QByteArray b;
        b.resize(1, 0);
        return b;
    }
    Value nv1 = *this;

    Value nv;

    // while (nv1 < 0)
    // {
    //     nv += 1;
    //     nv1 -= other;
    // }

    while ((nv1 -= other) >= 0)
    {
        // qDebug() << nv1.toHex() << nv.toHex();
        nv += 1;
    }
    // qDebug() << nv1.toHex() << (nv1 >= 0);

    return nv;
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
    Value nv;

    while (nv1 < 0)
    {
        nv += 1;
        nv1 -= other;
    }

    while ((nv1 -= other) >= 0)
        nv += 1;

    nv1.m_value.removeLast();

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
    return nv;
}

Value Value::operator>>(const Value& other) const
{
    Value count = other;
    Value nv = *this;
    bool flg = -(nv < 0);
    while (count != 0)
    {
        count -= 1;
        if (flg)
        {
            nv /= 2;
            auto& v = nv.m_value.back();
            v |= 0x80;
            continue;
        }
        nv /= 2;
    }
    return nv;
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

Value& Value::operator>>=(const Value& other)
{
    return *this = *this >> other;
}

/* ========================================================================== */

int Value::operator>(const Value& other) const
{
    auto v = *this - other;
    return -(v.m_value.back() > 0 || (v.m_value.back() == 0 && v.m_value.length() != 1));

    // QByteArray nb1, nb2;

    // int len = init_nb(nb1, nb2, this->m_value, other.m_value);

    // int flg = 0;

    // for (int i = len - 1; i >= 0; i--)
    // {
    //     if (nb1.at(i) != nb2.at(i))
    //     {
    //         flg = nb1.at(i) > nb2.at(i);
    //         break;
    //     }
    // }

    // return -flg;
}

int Value::operator>=(const Value& other) const
{
    return -(*this > other || *this == 0);
}

int Value::operator<(const Value& other) const
{
    return -(!(*this >= other));
    // auto v = *this - other;
    // return -(v.m_value.back() < 0);
}

int Value::operator<=(const Value& other) const
{
    return -(*this < other || *this == 0);
}

int Value::operator==(const Value& other) const
{
    auto v = *this - other;
    return -(v.m_value.back() == 0 && v.m_value.length() == 1);
}

int Value::operator!=(const Value& other) const
{
    auto v = *this - other;
    return -(v.m_value.back() != 0 || v.m_value.length() != 1);
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
            nb2.resize(len, nb2.back() < 0 ? 0xff : 0);
    }
    else
    {
        len = b2.length();
        if (nb1.isEmpty())
            nb1.resize(len, 0);
        nb1.resize(len, nb1.back() < 0 ? 0xff : 0);
    }
    return len;
}

void Value::least(QByteArray& b)
{
    if (b.isEmpty())
    {
        b.resize(1, 0);
        return;
    }
    // else if (b.back() & 0x80)
    // { // < 0
    //     while (!b.isEmpty() && b.back() == '\xff')
    //         b.removeLast();
    //     if (b.isEmpty())
    //         b.resize(1, '\xff');
    // }
    else
    {
        while (!b.isEmpty() && b.back() == 0)
        {
            b.removeLast();
            if (!b.isEmpty() && b.back() < 0)
            { // 修补正数变负数bug
                b.append('\0');
                break;
            }
        }
        if (b.isEmpty())
            b.resize(1, 0);
    }
}

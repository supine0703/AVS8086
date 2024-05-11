#ifndef VALUE_H
#define VALUE_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Value : public Expression
{
    friend class avs8086::parser::Parser; // 用作数据对齐
public:
    Value(const token::Token& token, size_t integer)
        : Expression(VALUE, token)
    {
        do
        {   m_value.append(integer & 0xff);
            integer >>= 8;
        } while (integer != 0);
        m_unitDataSize = m_value.length();
    }

    Value(const token::Token& token, const QByteArray& bytes)
        : Expression(VALUE, token)
        , m_value(bytes)
        , m_unitDataSize((Q_ASSERT(!bytes.isEmpty()), 1))
    { }

    virtual ~Value() = default;

    virtual QJsonObject json() const override;

    virtual QByteArray bytes() const override { return m_value; }

    virtual int dataSize() const override { return m_value.size(); }

    virtual int unitDataSize() const override { return m_unitDataSize; }

    size_t integer(bool* ok = nullptr) const
    {
        bool okey = m_value.size() < 8;
        if (ok != nullptr)
            *ok = okey;
        if (!okey)
            return 0;
        size_t v = 0;
        for (int i = m_value.size() - 1; i >= 0; --i)
        {
            v <<= 8;
            v |= m_value.at(i) & 0xff;
        }
        return v;
    }

#if 0
    // QByteArray 和 QString 相互转换可能会改变值, 因此弃用
    Value(const token::Token& token, const QString& string)
        : Expression(VALUE, token)
        , m_unitDataSize((Q_ASSERT(!string.isEmpty()), 1))
        // , m_value(string.toUtf8())
    {
        m_value.reserve(string.length() << 1);
        for (auto ch : string)
        {
            auto code = ch.unicode();
            m_value.append(code & 0xff);
            m_value.append((code >> 8) & 0xff);
        }
    }
    QString string() const
    {
        // return QString::fromUtf8(m_value);
        QString s;
        s.reserve(m_value.size());
        bool odd = m_value.size() & 1;
        for (int i = 1; i < m_value.size(); i += 2)
        {
            char16_t code = m_value.at(i) & 0xff;
            s.append(QChar(((code << 8) | (m_value.at(i - 1) & 0xff))));
        }
        if (odd)
            s.append(QChar(static_cast<char16_t>(m_value.back()) & 0xff));
        return s;
    }
#endif

protected:
    int m_unitDataSize = 0;
    QByteArray m_value;

    Value(Type type, const token::Token& token) : Expression(type, token) { }
};

/* ========================================================================== */

class Float : public Expression
{
public:
    Float(const token::Token& token)
        : Expression(FLOAT, token), m_value((*token).toDouble())
    { }

    ~Float() = default;

    virtual QJsonObject json() const override;

private:
    double m_value;
};

} // namespace avs8086::ast

#endif // VALUE_H

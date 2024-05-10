#ifndef ARRAY_H
#define ARRAY_H

#include "ast/node.h"

namespace avs8086::ast {

class Array : public Expression
{
public:
    virtual ~Array() = default;

    virtual QByteArray bytes() const override
    {
        QByteArray d;
        for (const auto& e : exprs())
        {
            d.append(e->bytes());
            auto mod = d.size() % m_unitSize;
            if (mod != 0)
            {
                d.append(m_unitSize - mod, 0);
            }
        }
        return d;
    }

    virtual int dataSize() const override
    {
        int s = 0;
        for (const auto& e : exprs())
        {
            s += e->dataSize();
            auto mod = s % m_unitSize;
            if (mod != 0)
            {
                s += m_unitSize - mod;
            }
        }
        return s;
    }

    virtual int unitDataSize() const override { return m_unitSize; }

protected:
    int m_unitSize;

    Array(Type type, const token::Token& token, int unitSize)
        : Expression(type, token), m_unitSize(unitSize)
    { }

    virtual const QList<ExprPointer>& exprs() const = 0;
};

} // namespace avs8086::ast

#endif // ARRAY_H

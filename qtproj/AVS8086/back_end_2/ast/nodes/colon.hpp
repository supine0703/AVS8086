#ifndef COLON_HPP
#define COLON_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Colon : public Expression
{
public:
    Colon(const QSharedPointer<Expression>& left)
        : Expression(NODE_COLON)
    {
        m_left = left;
        if (m_left->is(NODE_REGISTER))
        {
            m_value = "SREG";
        }
        else if (m_left->is(NODE_LABEL))
        {
            m_value = "LABEL";
        }
    }
    ~Colon() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(
            QString("%1| %2: '%3:'")
                .arg(QString(depth * 4, '-'), typeName(), m_value.toString())
            );
        info.append(m_left->traversal(depth + 1));
        if (m_right)
            info.append(m_right->traversal(depth + 1));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = m_value.toString() + typeName();
        QJsonObject exprs;
        js["statements"] = exprs;
        return js;
    }


public:
    QSharedPointer<Expression> m_left;
    QSharedPointer<Expression> m_right;
};

}; // namespace avs8086::ast

#endif // COLON_HPP

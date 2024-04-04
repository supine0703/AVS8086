#ifndef COMMA_HPP
#define COMMA_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Comma : public Expression
{
public:
    Comma(
        const QSharedPointer<Expression>& left,
        const QSharedPointer<Expression>& right
        )
        : Expression(NODE_COMMA)
    {
        if (left.isNull() || right.isNull())
        {
            goError();
            return;
        }
        m_expression.append(left);
        if (right->type() != NODE_COMMA)
        {
            m_expression.append(right);
        }
        else
        {
            m_expression.append(
                qSharedPointerDynamicCast<Comma>(right)->m_expression
            );
        }
    }
    ~Comma() { }

    QStringList traversal(int depth) const override
    {
        if (isError())
            return {
                QString("%1| %2: member pointer is null!")
                    .arg(QString(depth * 4, '-'), typeName())
            };
        QStringList info;
        info.append(
            QString("%1| %2: %3(%4)")
                .arg(QString(depth * 4, '-'), typeName(), ",")
                .arg(m_expression.length())
        );
        depth++;
        for (int i = 0; i < m_expression.length(); i++)
        {
            auto s = m_expression.at(i)->traversal(depth);
            s[0].replace("|", QString("| %1 -").arg(i + 1));
            info.append(s);
        }
        return info;
    }

    void append(const QSharedPointer<Expression>& expression)
    {
        if (expression->type() == NODE_COMMA)
        {
            m_expression.append(
                qSharedPointerDynamicCast<Comma>(expression)->m_expression
            );
        }
        else m_expression.append(expression);
    }

public:
    QList<QSharedPointer<Expression>> m_expression;
};

}; // namespace avs8086::ast

#endif // COMMA_HPP

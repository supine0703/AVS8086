#ifndef COMMA_HPP
#define COMMA_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Comma : public Expression
{
public:
    Comma(const QSharedPointer<Expression>& left,
          const QSharedPointer<Expression>& right)
        : Expression(NODE_COMMA)
    {
        m_expressions.append(left);
        m_expressions.append(right);
    }
    ~Comma() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(
            QString("%1| %2: %3(%4)")
                .arg(QString(depth * 4, '-'), typeName(), ",")
                .arg(m_expressions.length())
        );
        depth++;
        for (int i = 0; i < m_expressions.length(); i++)
        {
            auto s = m_expressions.at(i)->traversal(depth);
            s[0].replace("|", QString("| %1 -").arg(i + 1));
            info.append(s);
        }
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        QJsonObject exprs;
        for (const auto& e : m_expressions)
        {
            exprs[e->typeName()] = e->json();
        }
        js["expression"] = exprs;
        return js;
    }


public:
    void append(const QSharedPointer<Expression>& expression)
    {
        if (expression->isError())
        {
            goError();
        }
        if (expression->is(NODE_COMMA))
        {
            m_expressions.append(
                qSharedPointerDynamicCast<Comma>(expression)->m_expressions
            );
        }
        else
        {
            m_expressions.append(expression);
        }
    }

public:
    QList<QSharedPointer<Expression>> m_expressions;
};

}; // namespace avs8086::ast

#endif // COMMA_HPP

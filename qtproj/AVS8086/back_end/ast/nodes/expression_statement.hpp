#ifndef EXPRESSION_STATEMENT_HPP
#define EXPRESSION_STATEMENT_HPP

#include "ast/node.h"
#include <QSharedPointer>

namespace avs8086::ast {

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(const QSharedPointer<Expression>& expression)
        : Statement(NODE_EXPRESSION_STATEMENT), m_expression(expression)
    { }
    ~ExpressionStatement() { }
    
    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2").arg(
            QString(depth * 4, '-'), typeName()
        ));
        info.append(m_expression->traversal(depth + 1));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["statement"] = m_expression->json();
        return js;
    }

private:
    QSharedPointer<Expression> m_expression;
};

} // namespace avs8086::ast

#endif // EXPRESSION_STATEMENT_HPP

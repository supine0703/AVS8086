#ifndef EXPRESSION_STATEMENT_H
#define EXPRESSION_STATEMENT_H

#include "ast/node.h"
#include <QSharedPointer>

namespace avs8086::ast {

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(const QSharedPointer<Expression>& expression)
        : Statement(EXPRESSION_STATEMENT), m_expression(expression)
    { }
    ~ExpressionStatement() { }

    QJsonObject json() const override;

private:
    QSharedPointer<Expression> m_expression;
};

} // namespace avs8086::ast

#endif // EXPRESSION_STATEMENT_H

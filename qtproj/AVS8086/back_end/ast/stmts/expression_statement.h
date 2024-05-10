#ifndef EXPRESSION_STATEMENT_H
#define EXPRESSION_STATEMENT_H

#include "ast/node.h"
#include <QSharedPointer>

namespace avs8086::ast {

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement() : ExpressionStatement(ExprPointer(nullptr)) { }
    ExpressionStatement(const ExprPointer& expr)
        : Statement(EXPRESSION_STATEMENT), m_expr(expr)
    { }
    ~ExpressionStatement() { }

    virtual QJsonObject json() const override;

private:
    ExprPointer m_expr;
};

} // namespace avs8086::ast

#endif // EXPRESSION_STATEMENT_H

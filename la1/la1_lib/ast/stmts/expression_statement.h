#ifndef EXPRESSION_STATEMENT_H
#define EXPRESSION_STATEMENT_H

#include "ast/node.h"

#include <QSharedPointer>


namespace lzl::a1::ast {

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement() : ExpressionStatement(ExprPointer(nullptr)) {}
    ExpressionStatement(const ExprPointer& expr) : Statement(EXPR_STMT), m_expr(expr) {}
    ~ExpressionStatement() {}

    virtual QJsonObject json() const override;

private:
    ExprPointer m_expr;
};

} // namespace lzl::a1::ast

#endif // EXPRESSION_STATEMENT_H

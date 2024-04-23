#ifndef EXPRESSION_STATEMENT_HPP
#define EXPRESSION_STATEMENT_HPP

#include "ast/node.h"
#include "token/token.h"
#include <QSharedPointer>


class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(const Token& token = Token())
        : Statement(NODE_EXPRESSION_STATEMENT), m_token(token)
    { }
    ~ExpressionStatement() { }
    
    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), m_token.literal()
        ));
        if (m_expression != nullptr)
            info.append(m_expression->traversal(depth + 1));
        return info;
    }

    void setExpression(const QSharedPointer<Expression>& expression)
    {
        m_expression = expression;
    }

private:
    Token m_token;
    QSharedPointer<Expression> m_expression;
};

#endif // EXPRESSION_STATEMENT_HPP

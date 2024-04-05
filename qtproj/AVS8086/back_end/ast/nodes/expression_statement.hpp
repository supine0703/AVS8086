#ifndef EXPRESSION_STATEMENT_HPP
#define EXPRESSION_STATEMENT_HPP

#include "ast/node.h"

namespace avs8086::ast {

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(const QSharedPointer<Expression>& expression)
        : Statement(NODE_EXPRESSION_STATEMENT), m_expression(expression)
    {
        if (m_expression->isError())
            goError();
    }
    ~ExpressionStatement() { }
    
    QStringList traversal(int depth) const override
    {
        // if (isError())
        //     return {
        //         QString("%1| %2: member is false!")
        //             .arg(QString(depth * 4, '-'), typeName())
        //     };
        QStringList info;
        info.append(QString("%1| %2").arg(
            QString(depth * 4, '-'), typeName()
        ));
        info.append(m_expression->traversal(depth + 1));
        return info;
    }


private:
    QSharedPointer<Expression> m_expression;
};

} // namespace avs8086::ast

#endif // EXPRESSION_STATEMENT_HPP

#ifndef HLT_HPP
#define HLT_HPP

#include "ast/node.h"

namespace avs8086::ast {

class HLT : public Statement
{
public:
    HLT(const QSharedPointer<Expression>& expression)
        : Statement(NODE_MOV), m_expression(expression)
    {
        if (!m_expression.isNull()
            // && m_expression->type() == NODE_COMMA
            )
        {

        }
        else
            goError();
    }
    ~HLT() { }

    QStringList traversal(int depth) const override
    {
        if (isError())
            return {
                QString("%1| %2: member is false!")
                    .arg(QString(depth * 4, '-'), typeName())
            };
        QStringList info;
        info.append(QString("%1| %2: MOV").arg(
            QString(depth * 4, '-'), typeName()
            ));
        info.append(m_expression->traversal(depth + 1));
        return info;
    }


private:
    QSharedPointer<Expression> m_expression;
};

} // namespace avs8086::ast

#endif // HLT_HPP

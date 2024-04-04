#ifndef MOV_HPP
#define MOV_HPP

#include "ast/node.h"
// #include <QSharedPointer>

namespace avs8086::ast {

class MOV : public Statement
{
public:
    MOV(const QSharedPointer<Expression>& expression)
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
    ~MOV() { }

    QStringList traversal(int depth) const override
    {
        if (isError())
            return {
                QString("%1| %2: member pointer is null!")
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

#endif // MOV_HPP

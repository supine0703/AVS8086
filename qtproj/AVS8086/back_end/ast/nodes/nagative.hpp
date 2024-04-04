#ifndef NAGATIVE_HPP
#define NAGATIVE_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Nagative : public Expression
{
public:
    Nagative(const QSharedPointer<Expression>& m_expression = nullptr)
        : Expression(NODE_NAGATIVE) , m_expression(m_expression)
    {
        if (m_expression == nullptr)
            goError();
    }
    ~Nagative() { }

    QStringList traversal(int depth) const override
    {
        if (isError())
            return {
                QString("%1| %2: member pointer is null!")
                    .arg(QString(depth * 4, '-'), typeName())
            };
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), "-"
        ));
        info.append(m_expression->traversal(depth + 1));
        return info;
    }


private:
    QSharedPointer<Expression> m_expression;
};

} // namespace avs8086::ast

#endif // NAGATIVE_HPP

#ifndef PREFIX_H
#define PREFIX_H

#include "ast/node.h"

namespace avs8086::ast {

class Prefix : public Expression
{
public:
    Prefix(
        const QString& op,
        const QSharedPointer<Expression>& expression)
        : Expression(NODE_PREFIX)
        , m_operator(op)
        , m_expression(expression)
    {
        if (expression->isError())
        {
            m_valueType = ERROR;
            goError();
        }
        else if (op == "-" || op == "+" || op == "~")
        {
            m_valueType = expression->valueType();
            if (op == "-")
                m_value = -expression->value();
            else if (op == "~")
                m_value = ~expression->value();
        }
        else
        {
            m_valueType = ERROR;
            goError();
        }
    }
    ~Prefix() { }

    QStringList traversal(int depth) const override
    {
        // if (isError())
        //     return {
        //         QString("%1| %2: member is error!")
        //             .arg(QString(depth * 4, '-'), typeName())
        //     };
        QStringList info;
        info.append(QString("%1| %2: %3: '%4(%5)'").arg(
            QString(depth * 4, '-'), typeName(),
            isError() ? "error" : m_operator,
            m_value.toHex(' '), m_value.toString()
        ));
        info.append(m_expression->traversal(depth + 1));
        return info;
    }


private:
    QString m_operator;
    QSharedPointer<Expression> m_expression;
};

} // namespace avs8086::ast

#endif // PREFIX_H

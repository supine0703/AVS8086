#ifndef PREFIX_H
#define PREFIX_H

#include "ast/node.h"

namespace avs8086::ast {

class Prefix : public Expression
{
public:
    Prefix(
        const QString& op,
        const QSharedPointer<Expression>& m_expression = nullptr)
        : Expression(NODE_PREFIX)
        , m_operator(op)
        , m_expression(m_expression)
    {
        if (m_expression->isError()
            || !(op == "-" || op == "+" || op == "~"))
            goError();
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
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(),
            isError() ? m_operator : "error"
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

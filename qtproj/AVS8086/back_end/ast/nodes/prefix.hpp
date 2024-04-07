#ifndef PREFIX_HPP
#define PREFIX_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Prefix : public Expression
{
public:
    Prefix(
        const token::Token& token,
        const QSharedPointer<Expression>& expression)
        : Expression(NODE_PREFIX)
        , m_op(token.literal())
        , m_expression(expression)
    {
        m_token = token;
        if (expression->isError())
        {
            m_valueType = ERROR;
            goError();
        }
        else if (m_op == "-" || m_op == "+" || m_op == "~")
        {
            m_valueType = expression->valueType();
            if (m_op == "-")
                m_value = -expression->value();
            else if (m_op == "~")
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
        QStringList info;
        info.append(QString("%1| %2: %3: '%4(%5)'").arg(
            QString(depth * 4, '-'), typeName(),
            (isError() ? "error" : m_op),
            m_value.toHex(' '), m_value.toString()
        ));
        info.append(m_expression->traversal(depth + 1));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["m_op"] = m_op;
        js["hexValue"] = m_value.toHex(' ');
        js["strValue"] = m_value.toString();
        js["right"] = m_expression->json();
        return js;
    }

    QSharedPointer<Expression> m_expression;

private:
    QString m_op;
};

} // namespace avs8086::ast

#endif // PREFIX_HPP

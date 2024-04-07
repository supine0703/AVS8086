#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Address : public Expression
{
public:
    Address() : Expression(NODE_ADDRESS)
    {

    }
    ~Address() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: '%3(%4)'").arg(
            QString(depth * 4, '-'), typeName(),
            // isError() ? "error" : m_op,
            m_value.toHex(' '), m_value.toString()
            ));
        // info.append(m_expression->traversal(depth + 1));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        // js["op"] = m_op;
        js["hexValue"] = m_value.toHex(' ');
        js["strValue"] = m_value.toString();
        // js["right"] = m_expression->json();
        return js;
    }

    void setValue(Value v)
    {
        m_value = v;
    }

public:


};

} // namespace avs8086::ast

#endif // ADDRESS_HPP

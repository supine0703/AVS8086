#ifndef STRING_HPP
#define STRING_HPP

#include "ast/node.h"

namespace avs8086::ast {

class String : public Expression
{
public:
    String(const QString& str)
        : Expression(NODE_STRING)
    {
        m_valueType = STRING;
        m_value = str;
    }
    ~String() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %4(%5)").arg(
            QString(depth * 4, '-'), typeName(),
            m_value.toHex(' '), m_value.toString()
        ));
        return info;
    }
};

} // namespace avs8086::ast

#endif // STRING_H

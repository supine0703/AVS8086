#ifndef STRING_HPP
#define STRING_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class String : public Expression
{
public:
    String(const token::Token& token)
        : Expression(NODE_STRING)
    {
        m_value = token.literal().removeFirst().removeLast();
    }
    ~String() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), m_value.toString()
            ));
        return info;
    }
};

} // namespace avs8086::ast

#endif // STRING_H

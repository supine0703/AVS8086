#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Integer : public Expression
{
public:
    Integer(const token::Token& token)
        : Expression(NODE_INTEGER)
    {
        m_valueType = INTEGER;
        m_value = token::Token::textToInt(token.literal());
    }
    ~Integer() { }
    
    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(
            QString("%1| %2: %3")
                .arg(QString(depth * 4, '-'), typeName()).arg(m_value.toInt())
        );
        return info;
    }

};

} // namespace avs8086::ast

#endif // INTEGER_HPP

#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "ast/node.h"
#include "token/token.h"

class Integer : public Expression
{
public:
    Integer(const Token& token = Token())
        : Expression(NODE_INTEGER), m_token(token) { }
    ~Integer() { }
    
    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), m_token.literal()
        ));
        return info;
    }

    void setValue(int value)
    {
        m_value = value;
    }

private:
    Token m_token;
    int m_value;
};

#endif // INTEGER_HPP

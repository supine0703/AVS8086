#ifndef ILLEGAL_HPP
#define ILLEGAL_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Illegal : public Expression
{
public:
    Illegal(const token::Token& token = token::Token())
        : Expression(NODE_ILLEGAL), m_token(token)
    { goError(); }
    ~Illegal() { }

    QStringList traversal(int depth) const override
    {
        return {
            QString("%1| %2-error: %3 '%4'").arg(
                QString(depth * 4, '-'), typeName(),
                m_token.typeName(), m_token.literal()
            )
        };
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["tokenType"] = m_token.typeName();
        js["token"] = m_token.literal();
        return js;
    }

private:
    token::Token m_token;
};

} // namespace avs8086::ast

#endif // ILLEGAL_HPP

#ifndef ILLEGAL_H
#define ILLEGAL_H

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Illegal : public Expression
{
public:
    Illegal(const token::Token& token = token::Token())
        : Expression(ILLEGAL), m_token(token)
    { }
    ~Illegal() { }

    QJsonObject json() const override;

private:
    token::Token m_token;
};

} // namespace avs8086::ast

#endif // ILLEGAL_H

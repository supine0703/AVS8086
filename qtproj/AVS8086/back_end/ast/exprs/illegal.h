#ifndef ILLEGAL_H
#define ILLEGAL_H

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Illegal : public Expression
{
public:
    Illegal(const token::Token& t)
        : Expression(t.is(token::Token::TOKEN_EOL) ? NODE_EOL : ILLEGAL, t)
    { }
    ~Illegal() { }

    virtual QJsonObject json() const override;
};

} // namespace avs8086::ast

#endif // ILLEGAL_H

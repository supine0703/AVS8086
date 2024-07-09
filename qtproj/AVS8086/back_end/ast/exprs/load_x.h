#ifndef LOAD_X_H
#define LOAD_X_H

#include "ast/node.h"

namespace avs8086::ast {

class Load_X : public Expression
{
public:
    Load_X(const token::Token& token)
        : Expression(LOAD_X, token)
    { }

    ~Load_X() = default;

    virtual QJsonObject json() const override;

    QString value() const { return (*m_token).toUpper(); }

private:
};

} // namespace avs8086::ast

#endif // LOAD_X_H

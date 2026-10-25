#ifndef MAKE_X_H
#define MAKE_X_H

#include "ast/node.h"

namespace lzl::a1::ast {

class Make_X : public Expression
{
public:
    Make_X(const token::Token& token) : Expression(MAKE_X, token), m_value((*token).last(3).toUpper()) {}

    ~Make_X() = default;

    virtual QJsonObject json() const override;

    QString value() const { return m_value; }

private:
    QString m_value;
};

} // namespace lzl::a1::ast

#endif // MAKE_X_H

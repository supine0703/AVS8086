#ifndef ASSIGN_H
#define ASSIGN_H

#include "ast/node.h"

namespace avs8086::ast {

class Assign : public Expression
{
public:
    Assign(const token::Token& token)
        : Expression(ASSIGN, token)
    { }

    Assign(
        const token::Token& token,
        const ExprPointer& left,
        const ExprPointer& right)
        : Expression(ASSIGN, token)
        , m_left(left)
        , m_right(right)
    { }

    virtual QJsonObject json() const override;

    ExprPointer left() const { return m_left; }

    ExprPointer right() const { return m_right; }

private:
    ExprPointer m_left;
    ExprPointer m_right;
};

} // namespace avs8086::ast

#endif // ASSIGN_H

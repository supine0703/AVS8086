#ifndef COLON_H
#define COLON_H

#include "ast/node.h"

namespace avs8086::ast {

class Colon : public Expression
{
public:
    Colon(
        const token::Token& token,
        const ExprPointer& left,
        const ExprPointer& right)
        : Expression(COLON, token)
        , m_left(left)
        , m_right(right)
    { }

    ~Colon() = default;

    virtual QJsonObject json() const override;

    ExprPointer left() const { return m_left; }

    ExprPointer right() const { return m_right; }

private:
    ExprPointer m_left;
    ExprPointer m_right;
};

inline QJsonObject Colon::json() const
{
    QJsonObject js = Expression::json();
    js["left"] = m_left->json();
    js["right"] = m_right->json();
    return js;
}

} // namespace avs8086::ast

#endif // COLON_H

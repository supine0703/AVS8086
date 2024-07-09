#ifndef INFIX_H
#define INFIX_H

#include "ast/node.h"

namespace avs8086::ast {

class Infix : public Expression
{
public:
    using Pointer = QSharedPointer<Infix>;

    Infix(
        const token::Token& token,
        const ExprPointer& le,
        const ExprPointer& re = ExprPointer())
        : Expression(ILLEGAL, token), m_left(le), m_right(re)
    { }

    ~Infix() = default;

    virtual QJsonObject json() const override;

    virtual Position pos() const override
    { return m_left->pos() + m_right->pos(); }

    void resetLeft(const ExprPointer& expr) { m_left = expr; }

    void resetRight(const ExprPointer& expr) { m_left = expr; }

    ExprPointer left() const { return m_left; }

    ExprPointer right() const { return m_right; }

protected:
    ExprPointer m_left;
    ExprPointer m_right;
};

inline QJsonObject Infix::json() const
{
    QJsonObject js = Expression::json();
    if (!m_left.isNull())
    {
        js["left"] = m_left->json();
    }
    if (!m_right.isNull())
    {
        js["right"] = m_right->json();
    }
    return js;
}

} // namespace avs8086::ast

#endif // INFIX_H

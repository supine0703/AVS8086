#ifndef OPERATOR_H
#define OPERATOR_H

#include "ast/exprs/value.h"

namespace avs8086::ast {

class Operator : public Value
{
public:
    Operator(const token::Token& token, const ExprPointer& re)
        : Value(token), m_right((Q_ASSERT(!re.isNull()), re))
    { }

    Operator(
        const token::Token& token,
        const ExprPointer& le,
        const ExprPointer& re)
        : Value(token), m_left(le), m_right((Q_ASSERT(!re.isNull()), re))
    { }

    Operator(const Operator& op, size_t integer)
        : Value(op.m_token, integer)
        , m_left(op.m_left)
        , m_right(op.m_right)
    { }

    ~Operator() = default;

    virtual QJsonObject json() const override;

    virtual Position pos() const override
    { return m_left->pos() + m_right->pos(); }

    ExprPointer left() const { return m_left; }

    ExprPointer right() const { return m_right; }

private:
    ExprPointer m_left;
    ExprPointer m_right;
};

inline QJsonObject Operator::json() const
{
    QJsonObject js = Value::json();
    if (is(VALUE))
    {
        js["type(expr)"] = "Operator Value";
    }
    if (!m_left.isNull())
    {
        js["left"] = m_left->json();
    }
    js["right"] = m_right->json();
    return js;
}

} // namespace avs8086::ast

#endif // OPERATOR_H

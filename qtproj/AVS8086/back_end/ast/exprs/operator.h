#ifndef OPERATOR_H
#define OPERATOR_H

#include "ast/exprs/value.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Operator : public Value
{
    friend class avs8086::parser::Parser;
public:
    Operator(const token::Token& token, const ExprPointer& right)
        : Value(OPERATOR, token), m_right(right)
    { }

    Operator(
        const token::Token& token,
        const ExprPointer& left,
        const ExprPointer& right)
        : Value(OPERATOR, token), m_left(left), m_right(right)
    { }

    ~Operator() = default;

    virtual QJsonObject json() const override;

private:
    ExprPointer m_left;
    ExprPointer m_right;
};

inline QJsonObject Operator::json() const
{
    QJsonObject js = Value::json();
    js["op"] = *m_token;
    if (!m_left.isNull())
        js["left"] = m_left->json();
    js["right"] = m_right->json();
    return js;
}

} // namespace avs8086::ast

#endif // OPERATOR_H

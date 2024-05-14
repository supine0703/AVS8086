#ifndef DUP_H
#define DUP_H

#include "ast/exprs/array.h"

namespace avs8086::ast {

class Dup : public Array
{
public:
    Dup(
        const token::Token& token,
        const ExprPointer& left,
        const ExprPointer& right,
        int unitSize = 0,
        int count = 0)
        : Array(DUP, token, unitSize)
        , m_left(left)
        , m_right((Q_ASSERT(!right.isNull()), right))
        , m_exprs(count, right)
    { }

    ~Dup() = default;

    virtual QJsonObject json() const override;

    ExprPointer left() const { return m_left; }

    ExprPointer right() const { return m_right; }

protected:
    virtual const QList<ExprPointer>& exprs() const override
    { return m_exprs; }

private:
    ExprPointer m_left;
    ExprPointer m_right;
    QList<ExprPointer> m_exprs;
};

inline QJsonObject Dup::json() const
{
    QJsonObject js = Expression::json();
    js["left"] = m_left->json();
    js["right"] = m_right->json();
    js["data"] = QString(show_Integer_hex(bytes()));
    return js;
}

} // namespace avs8086::ast

#endif // DUP_H

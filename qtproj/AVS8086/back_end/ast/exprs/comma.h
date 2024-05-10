#ifndef COMMA_H
#define COMMA_H

#include "ast/exprs/array.h"

namespace avs8086::ast {

class CommaArray;

class Comma : public Expression
{
    friend class avs8086::ast::CommaArray;
public:
    Comma(
        const token::Token& token,
        const ExprPointer& left,
        const ExprPointer& right)
        : Expression(COMMA, token)
    {
        left->addIn(left, m_exprs);
        right->addIn(right, m_exprs);
    }
    ~Comma() { }

    virtual QJsonObject json() const override;

    virtual void addIn(const ExprPointer& e, QList<ExprPointer>& exprs) override
    {
        if (e->is(COMMA))
            exprs.append(m_exprs);
        else
            Expression::addIn(e, exprs);
    }

    QList<ExprPointer> expressions() const { return m_exprs; }

private:
    QList<ExprPointer> m_exprs;
};

inline QJsonObject Comma::json() const
{
    QJsonObject js = Expression::json();
    QJsonObject exprs;
    for (int i = 0; i < m_exprs.count(); i++)
    {
        const auto& e = m_exprs.at(i);
        exprs[QString("expr %1").arg(i + 1)] = e->json();
    }
    js["expressions"] = exprs;
    return js;
}

/* ========================================================================== */

class CommaArray : public Array
{
public:
    CommaArray(const QSharedPointer<Comma>& comma, int unitSize)
        : Array(COMMA_ARRAY, comma->token(), unitSize)
        , m_comma(comma)
    { }

    ~CommaArray() = default;

    virtual QJsonObject json() const override;

protected:
    virtual const QList<ExprPointer>& exprs() const override
    { return m_comma->m_exprs; }

private:
    QSharedPointer<Comma> m_comma;
};

inline QJsonObject CommaArray::json() const
{
    QJsonObject js = Expression::json();
    js["Comma"] = m_comma->json();
    js["data"] = QString(show_Integer_hex(bytes()));
    return js;
}

}; // namespace avs8086::ast

#endif // COMMA_H

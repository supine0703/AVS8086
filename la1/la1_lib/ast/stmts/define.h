#ifndef DEFINE_H
#define DEFINE_H

#include "ast/node.h"

namespace lzl::a1::parser {
class Parser;
} // namespace lzl::a1::parser

namespace lzl::a1::ast {

class Define : public Statement
{
    friend class lzl::a1::parser::Parser;

public:
    Define(const QString& id, const token::Token& op) : Statement(DEFINE), m_id(id), m_op(op) {}

    ~Define() = default;

    virtual QJsonObject json() const override;

    virtual void addIn(const StmtPointer& s, QList<StmtPointer>& stmts) override
    {
        Statement::addIn(s, stmts);
        if (!m_stmt.isNull())
        {
            m_stmt->addIn(m_stmt, stmts);
        }
    }

    virtual QByteArray code() const override
    {
        if (!m_expr.isNull())
        {
            return m_expr->bytes();
        }
        return Statement::code();
    }

private:
    QString m_id;
    token::Token m_op;
    StmtPointer m_stmt;
    ExprPointer m_expr;
};

inline QJsonObject Define::json() const
{
    QJsonObject js = Statement::json();
    js["id"] = m_id;
    js["op"] = m_op.content();
    if (!m_expr.isNull())
    {
        js["expr"] = m_expr->json();
        if (m_expr->unitDataSize() != 0)
        {
            js["code"] = QString(show_integer_hex(m_expr->bytes()));
        }
    }
    return js;
}

} // namespace lzl::a1::ast

#endif // DEFINE_H

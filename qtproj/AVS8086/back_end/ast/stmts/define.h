#ifndef DEFINE_H
#define DEFINE_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Define : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Define(const QString& id, const token::Token& op)
        : Statement(DEFINE), m_id(id), m_op(op)
    { }

    ~Define() = default;

    virtual QJsonObject json() const override;

    virtual void addIn(const StmtPointer& s, QList<StmtPointer>& stmts) override
    {
        Statement::addIn(s, stmts);
        if (!m_stmt.isNull())
            m_stmt->addIn(m_stmt, stmts);
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
    }
    return js;
}

} // namespace avs8086::ast

#endif // DEFINE_H

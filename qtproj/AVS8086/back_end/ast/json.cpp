#include "ast/expr_headers.h"
#include "ast/stmt_headers.h"

using namespace avs8086::ast;

#define JSON(X) QJsonObject X::json() const

/* ========================================================================== */

JSON(Expression)
{
    QJsonObject js;
    js["type(expr)"] = typeName();
    js["token"] = m_token.content();
    return js;
}

JSON(Statement)
{
    QJsonObject js;
    js["type(stmt)"] = typeName();
    if (!m_codes.isEmpty())
    {
        js["code"] = QString::fromUtf8(m_codes.toHex());
    }
    return js;
}

/* ========================================================================== */

JSON(Program)
{
    QJsonObject js = Statement::json();
    js["file"] = m_file;
#if 0
    QJsonObject symbols;
    for (const auto& id : m_identifiers.list())
    {
        QJsonObject id_js;
        id_js["offset"] = id.offset();
        id_js["size"] = id.size();
        id_js["type"] = id.typeName();
        id_js["segment"] = id.segment();
        symbols[id.id()] = id_js;
    }
js["symbols"] = symbols;
#endif
    QJsonObject stmts;
    for (int i = 0; i < m_stmts.size(); i++)
    {
        stmts[QString("stmt %1").arg(i + 1)] = m_stmts.at(i)->json();
    }
    js["statements"] = stmts;
    return js;
}

/* ========================================================================== */

JSON(ExpressionStatement)
{
    QJsonObject js = Statement::json();
    js["statement"] = m_expr->json();
    return js;
}

/* ========================================================================== */

JSON(Illegal)
{
    QJsonObject js = Expression::json();
    return js;
}

/* ========================================================================== */

JSON(Well)
{
    QJsonObject js = Statement::json();
    js["expr"] = m_expr->json();
    return js;
}

/* ========================================================================== */

JSON(Value)
{
    QJsonObject js = Expression::json();
    js["value"] = "0x" + QString(show_Integer_hex(m_value));
    return js;
}

JSON(Float)
{
    QJsonObject js = Expression::json();
    return js;
}

/* ========================================================================== */

JSON(Register)
{
    QJsonObject js = Expression::json();
    return js;
}

/* ========================================================================== */

JSON(Make_X)
{
    QJsonObject js = Expression::json();
    js["MAKE"] = m_value;
    return js;
}

/* ========================================================================== */

JSON(Load_X)
{
    QJsonObject js = Expression::json();
    return js;
}

/* ========================================================================== */

JSON(Assign)
{
    QJsonObject js;
    js["type(expr)"] = typeName();
    js["op"] = *m_token;
    js["left"] = m_left->json();
    js["right"] = m_right->json();
    return js;
}

/* ========================================================================== */


/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



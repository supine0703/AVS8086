#include "node.h"

using namespace avs8086::ast;

#define JSON(X) QJsonObject X::json() const

/* ========================================================================== */

#include "ast/nodes/program.h"
JSON(Program)
{
    QJsonObject js;
    js["type"] = typeName();
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
    for (int i = 0; i < m_statements.length(); i++)
    {
        const auto& s = m_statements.at(i);
        stmts[QString("stmt %1").arg(i + 1)] = s->json();
    }
    js["statements"] = stmts;
    return js;
}

/* ========================================================================== */

#include "ast/nodes/expression_statement.h"
JSON(ExpressionStatement)
{
    QJsonObject js;
    js["type"] = typeName();
    js["statement"] = m_expression->json();
    return js;
}

/* ========================================================================== */

#include "ast/nodes/illegal.h"
JSON(Illegal)
{
    QJsonObject js;
    js["type"] = typeName();
    js["tokenType"] = m_token.typeName();
    js["token"] = m_token.literal();
    return js;
}

/* ========================================================================== */

#include "ast/nodes/well.h"
JSON(Well)
{
    QJsonObject js;
    js["type"] = typeName();
    js["instruction"] = m_instruction;
    return js;
}

/* ========================================================================== */

#include "ast/nodes/identifier.h"
JSON(Variable)
{
    QJsonObject js;

    js["type"] = typeName();
    return js;
}

/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



/* ========================================================================== */



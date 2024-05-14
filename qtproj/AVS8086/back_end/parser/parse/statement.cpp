#include "parser/parser.h"
#include "ast/stmts/expression_statement.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_statement()
{
    Q_ASSERT(!currToken().is(Token::TOKEN_EOF));
    if (currToken().is(Token::ILLEGAL) || currToken().is(Token::TOKEN_EOL))
        return StmtPointer(nullptr);
    StmtPointer s;
    auto c_it = sm_stmt_parseFns.find(currToken().type());
    if (c_it != sm_stmt_parseFns.end())
        s = (this->*c_it.value())();
    else
    {
        auto p_it = sm_post_parseFns.find(peekToken().type());
        if (p_it != sm_post_parseFns.end())
            s = (this->*p_it.value())();
        else
        {
            auto e = parse_expression();
            addInfo(
                Info::ERROR, e->pos(),
                "cannot use expression as statement"
            );
            s = StmtPointer(new ExpressionStatement(e));
        }
    }
    return s;
}

/* ========================================================================== */

StmtPointer Parser::parse_expression_statement()
{
    auto e = parse_expression();
    // Q_ASSERT(!e.isNull());
    StmtPointer s(new ExpressionStatement(e));
    return s;
}

/* ========================================================================== */

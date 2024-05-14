#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_group()
{
    auto t_pos = currToken().pos();

    nextToken();
    if (currToken().is(Token::RPAREN))
    {
        addInfo(
            Info::ERROR, t_pos + currToken().pos(),
            "empty in '()'"
        );
        return parse_illegal(currToken());
    }

    auto e = parse_expression();

    if (peekToken().is(Token::TOKEN_EOL))
    {
        addInfo(
            Info::ERROR, t_pos + currToken().pos(),
            "'(' is not close on this line"
        );
    }
    else if (!expectPeekToken(true, Token::RPAREN))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::RPAREN});
    }
    return e;
}

/* ========================================================================== */

#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

ExprPointer Parser::parse_group()
{
    int row = currToken().row();
    int col = currToken().column();
    nextToken();
    auto e = parse_expression();

    if (peekToken().is(Token::TOKEN_EOL))
    {
        addInfo(Info::ERROR, {row, col, currToken().column() - col},
                "'(' is not close on this line");
    }
    else if (!expectPeekToken(true, Token::RPAREN))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::RPAREN});
    }
    return e;
}

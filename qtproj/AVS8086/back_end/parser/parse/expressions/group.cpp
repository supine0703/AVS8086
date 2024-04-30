#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

ExprPointer Parser::parse_group()
{
    int row = currToken().row();
    int col = currToken().column();
    nextToken();
    auto e = parse_expression();
    if (expectPeekToken(Token::TOKEN_EOF, false))
    {
        addErrorInfo(row, col, currToken().column() - col,
                     "'(' is not close on this line");
    }
    else
    {
        expectPeekToken(Token::RPAREN);
    }
    return e;
}

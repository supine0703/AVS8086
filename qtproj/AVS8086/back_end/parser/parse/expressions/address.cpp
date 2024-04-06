#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_address()
{
    int row = currToken().row();
    int col = currToken().column();
    nextToken();
    auto e = parse_expression(LOWEST);
    int _r = peekToken().row();
    if (row != _r)
    {
        addErrorInfo(row, col, 1, "'[' is not close on this line");
    }
    else if (!expectPeekToken(Token::TOKEN_RSQUARE))
    {
        addExpectPeekTokenErrorInfo(Token::TOKEN_RSQUARE);
    }
    return e;
}

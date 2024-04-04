#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_group()
{
    int row = currToken().row();
    int col = currToken().column();
    nextToken();
    auto e = parse_expression(LOWEST);
    int _r = peekToken().row();
    if (row != _r || !expectPeekToken(Token::TOKEN_RPAREN))
    {
        addErrorInfo(row, col, 1, "'(' is not close on this line");
        return nullptr;
    }
    return e;
}

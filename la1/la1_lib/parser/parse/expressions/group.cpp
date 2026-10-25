#include "parser/parser.h"

using namespace lzl::a1::ast;
using namespace lzl::a1::token;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_group()
{
    int row = currToken().row();
    int col = currToken().column();
    nextToken();
    auto e = parse_expression(LOWEST);
    if (peekToken().is(Token::TOKEN_EOF))
    {
        addErrorInfo(row, col, 1, "'(' is not close on this line");
    }
    else if (!expectPeekToken(Token::TOKEN_RPAREN))
    {
        addExpectPeekTokenErrorInfo(Token::TOKEN_RPAREN);
    }
    return e;
}

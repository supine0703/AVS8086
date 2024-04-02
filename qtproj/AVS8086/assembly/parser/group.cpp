#include "parser/parser.h"


QSharedPointer<Expression> Parser::parse_group()
{
    nextToken();
    auto e = parse_expression(LOWEST);
    if (!expectPeekToken(Token::TOKEN_RPAREN))
    {
        return nullptr;
    }
    return e;
}

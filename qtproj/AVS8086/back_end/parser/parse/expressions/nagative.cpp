#include "parser/parser.h"
#include "ast/nodes/nagative.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_nagative()
{
    nextToken();
    return QSharedPointer<Nagative>(new Nagative(
        parse_expression(tokenPrecedence(Token::TOKEN_NEGATIVE))
    ));
}

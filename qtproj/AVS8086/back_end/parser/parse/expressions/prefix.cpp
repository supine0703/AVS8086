#include "parser/parser.h"
#include "ast/nodes/prefix.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_prefix()
{
    QString op = currToken().literal();
    nextToken();
    return QSharedPointer<Prefix>(new Prefix(
        op, parse_expression(tokenPrecedence(Token::TOKEN_NEGATIVE))
    ));
}

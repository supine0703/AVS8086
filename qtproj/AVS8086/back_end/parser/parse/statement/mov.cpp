#include "parser/parser.h"
#include "ast/nodes/mov.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Statement> Parser::parse_mov()
{
    nextToken();
    return QSharedPointer<Statement>(new MOV(parse_expression(LOWEST)));
}

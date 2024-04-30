#include "parser/parser.h"
#include "ast/nodes/illegal.h"
using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_illegal()
{
    return QSharedPointer<Expression>(new Illegal(currToken()));
}

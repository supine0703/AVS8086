#include "parser/parser.h"
#include "ast/nodes/register.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_register()
{
    return QSharedPointer<Expression>(new Register(currToken()));
}

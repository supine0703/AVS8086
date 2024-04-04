#include "parser/parser.h"
#include "ast/nodes/integer.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_integer()
{
    return QSharedPointer<Integer>(new Integer(currToken()));
}

#include "parser/parser.h"
#include "ast/nodes/single.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Statement> Parser::parse_single()
{
    nextToken();
    return QSharedPointer<Statement>(new Single(currToken()));
}

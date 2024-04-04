#include "parser/parser.h"
#include "ast/nodes/string.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_string()
{
    return QSharedPointer<String>(new String(currToken()));
}

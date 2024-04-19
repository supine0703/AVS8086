#include "parser/parser.h"
#include "ast/nodes/illegal.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_reserved_word()
{
    addReservedWordErrorInfo();
    return QSharedPointer<Expression>(new Illegal(currToken()));
}

#include "parser/parser.h"
#include "ast/nodes/float.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_float()
{
    addNoDealingWithTokenErrorInfo();
    return QSharedPointer<Float>(new Float(currToken()));
}


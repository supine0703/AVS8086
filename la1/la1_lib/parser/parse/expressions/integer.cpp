#include "ast/nodes/integer.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_integer()
{
    return QSharedPointer<Expression>(new Integer(currToken()));
}

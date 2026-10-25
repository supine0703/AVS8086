#include "ast/nodes/register.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_register()
{
    return QSharedPointer<Expression>(new Register(currToken()));
}

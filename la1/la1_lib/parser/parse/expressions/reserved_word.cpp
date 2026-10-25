#include "ast/nodes/illegal.hpp"
#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_reserved_word()
{
    addReservedWordErrorInfo();
    return QSharedPointer<Expression>(new Illegal(currToken()));
}

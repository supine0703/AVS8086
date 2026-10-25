#include "ast/nodes/infix.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::token;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_infix(const QSharedPointer<Expression>& left)
{

    auto precedence = currTokenPrecedence();
    Token t = currToken();
    nextToken();
    return QSharedPointer<Expression>(new Infix(left, parse_expression(precedence), t));
}

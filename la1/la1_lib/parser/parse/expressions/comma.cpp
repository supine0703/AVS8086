#include "ast/nodes/comma.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::token;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_comma(const QSharedPointer<ast::Expression>& left)
{
    auto precedence = currTokenPrecedence();
    nextToken();
    return QSharedPointer<Expression>(new Comma(left, parse_expression(precedence)));
}

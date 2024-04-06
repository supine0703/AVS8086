#include "parser/parser.h"
#include "ast/nodes/comma.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression>
Parser::parse_comma(const QSharedPointer<ast::Expression>& left)
{
    auto precedence = currTokenPrecedence();
    nextToken();
    return QSharedPointer<Expression>(
        new Comma(left, parse_expression(precedence))
    );
}

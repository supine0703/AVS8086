#include "parser/parser.h"
#include "ast/nodes/infix.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression>
Parser::parse_infix(const QSharedPointer<Expression>& left)
{

    auto precedence = currTokenPrecedence();
    Token t = currToken();
    nextToken();
    return QSharedPointer<Expression>(
        new Infix(left, parse_expression(precedence), t)
    );
}

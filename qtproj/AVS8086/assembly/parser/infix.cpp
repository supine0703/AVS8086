#include "parser/parser.h"
#include "ast/node/infix.hpp"

QSharedPointer<Expression>
Parser::parse_infix(const QSharedPointer<Expression>& left)
{
    QSharedPointer<Infix> e(new Infix(currToken()));
    e->setOperator(currToken().literal());
    e->setLeft(left);
    auto precedence = currTokenPrecedence();
    nextToken();
    e->setRight(parse_expression(precedence));
    return e;
}

#include "parser/parser.h"
#include "ast/node/expression_statement.hpp"


QSharedPointer<Statement> Parser::parse_statement()
{
    return parse_expression_statement();
}

QSharedPointer<ExpressionStatement> Parser::parse_expression_statement()
{
    QSharedPointer<ExpressionStatement> s(new ExpressionStatement(currToken()));
    s->setExpression(parse_expression(LOWEST));
    nextToken();
    return s;
}

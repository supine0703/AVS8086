#include "parser/parser.h"
#include "ast/nodes/expression_statement.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Statement> Parser::parse_statement()
{
    auto it = sm_statement_parseFns.find(currToken().type());
    if (it != sm_statement_parseFns.end())
        return (this->*it.value())();
    return parse_expression_statement();
}

QSharedPointer<ExpressionStatement> Parser::parse_expression_statement()
{
    return QSharedPointer<ExpressionStatement>(
        new ExpressionStatement(parse_expression(LOWEST))
    );
}

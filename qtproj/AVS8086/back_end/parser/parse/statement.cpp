#include "parser/parser.h"
#include "ast/nodes/expression_statement.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Statement> Parser::parse_statement()
{
    QSharedPointer<Statement> s;
    auto it = sm_stmt_parse_fns.find(currToken().type());
    if (it != sm_stmt_parse_fns.end())
        s = (this->*it.value())();
    else
        s = parse_expression_statement();
    return s;
}

QSharedPointer<ExpressionStatement> Parser::parse_expression_statement()
{
    auto s = QSharedPointer<ExpressionStatement>(
        new ExpressionStatement(parse_expression(LOWEST))
    );

    return s;
}

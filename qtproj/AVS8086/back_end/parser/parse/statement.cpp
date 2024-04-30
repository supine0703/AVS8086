#include "parser/parser.h"
#include "ast/nodes/expression_statement.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_statement()
{
    if (currToken().is(Token::ILLEGAL))
        return StmtPointer(nullptr);
    StmtPointer s;
    auto it = sm_stmt_parseFns.find(currToken().type());
    if (it != sm_stmt_parseFns.end())
        s = (this->*it.value())();
    else
    {
        int row = currToken().row();
        int col = currToken().column();
        s = parse_expression_statement();
        int len = currToken().column() - col;
        addErrorInfo(row, col, len, "can not be used expression statement");
    }
    return s;
}

StmtPointer Parser::parse_expression_statement()
{
    StmtPointer s = QSharedPointer<ExpressionStatement>(
        new ExpressionStatement(parse_expression())
    );
    return s;
}

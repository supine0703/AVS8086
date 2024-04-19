#include "parser/parser.h"
#include "ast/nodes/prefix.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_prefix()
{
    int row = currToken().row();
    int col = currToken().column();
    auto curr = currToken();
    nextToken();
    auto right = parse_expression(PREFIX);
    int len = currToken().column() + currToken().literal().length() - col;

    if (!(right->valueIs(Expression::STRING)
          || right->valueIs(Expression::INTEGER)))
    {
        addErrorInfo(
            row, col, len,
            QString("'%1' expect string or integer").arg(curr.literal())
        );
    }

    return QSharedPointer<Expression>(new Prefix(curr, right));
}

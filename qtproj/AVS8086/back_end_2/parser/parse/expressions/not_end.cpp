#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_not_end()
{
    const auto& t = currToken();
    addErrorInfo(
        t.row() - 1, t.column(), t.literal().length(), "expression is not end"
    );
    return parse_illegal();
}

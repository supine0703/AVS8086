#include "parser/parser.h"

using namespace lzl::a1::ast;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_not_end()
{
    const auto& t = currToken();
    addErrorInfo(t.row() - 1, t.column(), t.literal().length(), "expression is not end");
    return parse_illegal();
}

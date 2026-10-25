#include "ast/exprs/illegal.h"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::token;
using namespace lzl::a1::parser;

/* ========================================================================== */

ExprPointer Parser::parse_illegal(const token::Token& token)
{
    return ExprPointer(new Illegal(token));
}

/* ========================================================================== */

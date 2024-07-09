#include "parser/parser.h"
#include "ast/exprs/illegal.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_illegal(const token::Token& token)
{
    return ExprPointer(new Illegal(token));
}

/* ========================================================================== */

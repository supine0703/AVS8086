#include "parser/parser.h"
#include "ast/stmts/single.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_single()
{
    auto s = Single::NEW(currToken().type());
    Q_ASSERT(!s.isNull());
    if (currToken().is(Token::LOCK))
    {
        addReservedWordWarningInfo();
    }
    return s;
}

/* ========================================================================== */

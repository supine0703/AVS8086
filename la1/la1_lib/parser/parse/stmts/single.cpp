#include "ast/stmts/single.h"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::token;
using namespace lzl::a1::parser;

/* ========================================================================== */

StmtPointer Parser::parse_single()
{
#if 0
    auto s = Single::NEW(currToken().type());
    Q_ASSERT(!s.isNull());
#endif
    Single::Pointer s(new Single(currToken().type()));
    if (currToken().is(Token::LOCK))
    {
        addReservedWordWarningInfo();
    }
    return s;
}

/* ========================================================================== */

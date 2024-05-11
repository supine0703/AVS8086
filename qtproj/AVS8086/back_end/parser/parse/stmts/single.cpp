#include "parser/parser.h"
#include "ast/stmts/single.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_single()
{
    m_currOffset += 1;
    StmtPointer s(new Single(currToken()));
    nextToken();
    return s;
}

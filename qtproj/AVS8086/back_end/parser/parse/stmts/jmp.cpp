#include "parser/parser.h"
#include "ast/stmts/jmp.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_jmp()
{
    StmtPointer s(new Jmp(peekToken()));
    nextToken(); // 检错包含在 callId(s) 中了
    callId(s);
    return s;
}

StmtPointer Parser::parse_jx()
{
    StmtPointer s(new Jx(currToken(), peekToken()));
    nextToken();
    callId(s);
    return s;
}


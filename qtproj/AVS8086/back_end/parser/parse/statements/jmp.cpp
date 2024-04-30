#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_jmp()
{
    int row = currToken().row();
    int col = currToken().column();
    int len = peekToken().endColumn() - col;
    QString id = currToken().literal();
    if (expectPeekToken(Token::COLON))
    {
        if (expectPeekTokenNot(Token::TOKEN_EOF))
            return parse_statement();
    }
    return StmtPointer(nullptr);
}

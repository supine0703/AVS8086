#include "parser/parser.h"


using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

ExprPointer Parser::parse_address()
{
    int row = currToken().row();
    int col = currToken().column();

    nextToken();
    if (currToken().is(Token::RSQUARE))
    {
        addInfo(
            Info::ERROR, {row, col, currToken().endColumn() - col},
            "empty in '[]'"
        );
        return parse_illegal(currToken());
    }

    auto e = parse_expression();

    if (peekToken().is(Token::TOKEN_EOL))
    {
        addInfo(
            Info::ERROR, {row, col, currToken().endColumn() - col},
            "'[' is not close on this line"
        );
    }
    else if (!expectPeekToken(true, Token::RSQUARE))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::RSQUARE});
    }


}

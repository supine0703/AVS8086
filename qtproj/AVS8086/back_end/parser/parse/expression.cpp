#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

ExprPointer Parser::parse_expression(Precedence precedence)
{
    auto prefix = sm_prefix_parseFns.find(currToken().type());
    if (prefix == sm_prefix_parseFns.end())
    {
        addNoPrefixParseFnErrorInfo();
        return parse_illegal();
    }

    auto e = (this->*prefix.value())();

    while (precedence < peekTokenPrecedence())
    {
        auto infix = sm_infix_parseFns.find(peekToken().type());
        if (infix == sm_infix_parseFns.end())
        { // not infix, only prefix
            return e;
        }
        else
        { // infix
            nextToken();
            e = (this->*infix.value())(e);
        }
    }

    Q_ASSERT(!e.isNull());
    return e;
}


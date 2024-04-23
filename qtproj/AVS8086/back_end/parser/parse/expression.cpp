#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_expression(Precedence precedence)
{
    auto prefix = sm_prefix_parse_fns.find(currToken().type());
    if (prefix == sm_prefix_parse_fns.end())
    {
        // addNoPrefixParseFnErrorInfo();
        return parse_illegal();
    }

    QSharedPointer<Expression> e = (this->*prefix.value())();

    while (precedence < peekTokenPrecedence())
    {
        auto infix = sm_infix_parse_fns.find(peekToken().type());
        if (infix == sm_infix_parse_fns.end())
        { // not infix, only prefix
            return e;
        }
        else
        { // infix
            nextToken();
            e = (this->*infix.value())(e);
        }
    }
    return e;
}


#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_expression(Precedence precedence)
{
    auto prefix = sm_prefix_parseFns.find(currToken().type());
    if (prefix == sm_prefix_parseFns.end())
    {
        addNoPrefixParseFnErrorInfo();
        return nullptr;
    }

    QSharedPointer<Expression> e = (this->*prefix.value())();

    while (precedence < peekTokenPrecedence())
    {
        auto postfix = sm_postfix_parseFns.find(peekToken().type());
        if (postfix == sm_postfix_parseFns.end())
        { // not postfix
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
        else
        { // postfix
            nextToken();
            e = (this->*postfix.value())(e);
        }
    }
    return e;
}


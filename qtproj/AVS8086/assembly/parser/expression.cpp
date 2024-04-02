#include "parser/parser.h"


QSharedPointer<Expression> Parser::parse_expression(Precedence precedence)
{
    if (!sm_prefix_parseFns.contains(currToken().type()))
    {
        addNoPrefixParseFnErrorInfo();
        return nullptr;
    }

    auto prefix = sm_prefix_parseFns.value(currToken().type());
    QSharedPointer<Expression> e = (this->*prefix)();
    while (precedence < peekTokenPrecedence())
    {
        if (sm_postfix_parseFns.contains(peekToken().type()))
        {
            nextToken();
            auto postfix = sm_postfix_parseFns.value(currToken().type());
            e = (this->*postfix)(e);
        }
        else if (sm_infix_parseFns.contains(peekToken().type()))
        {
            nextToken();
            auto infix = sm_infix_parseFns.value(currToken().type());
            e = (this->*infix)(e);
        }
        else
            return e;
    }
    return e;
}


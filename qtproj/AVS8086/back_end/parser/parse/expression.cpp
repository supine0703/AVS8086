#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_expression(Precedence precedence)
{
    if (currToken().is(Token::TOKEN_EOL))
    {
        return parse_illegal(currToken()); // eol
        // qDebug() << currToken().row() << currToken().column() << *currToken();
    }

    if (sm_stmt_parseFns.contains(currToken().type())
        || sm_post_parseFns.contains(currToken().type()))
    {
        addStmtCannotBeExprErrorInfo();
        return parse_illegal(currToken());
    }

    auto prefix = sm_prefix_parseFns.find(currToken().type());
    if (prefix == sm_prefix_parseFns.end())
    {
        addNoPrefixParseFnErrorInfo();
        return parse_illegal(currToken());
    }

    auto e = (this->*prefix.value())();
    Q_ASSERT(!e.isNull());

    while (precedence < peekPrecedence())
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
            Q_ASSERT(!e.isNull());
        }
    }

    return e;
}

/* ========================================================================== */

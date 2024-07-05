#include "parser/parser.h"
#include "ast/stmts/push_pop.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_push_pop()
{
    Q_ASSERT(currToken().is(Token::PUSH) || currToken().is(Token::POP));
    bool isPush = currToken().is(Token::PUSH);

    auto pp(isPush ? PushPop::Pointer(new Push) : PushPop::Pointer(new Pop));

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        addExpectExprErrorInfo(eol);
        pp->resetExpr(eol);
        return pp;
    }

    auto e = parse_expression();
    pp->resetExpr(e);

    if (e->is(Node::REGISTER))
    {
        auto reg = assert_dynamic_cast<Register>(e);
        if (reg->token().is(Token::REG8))
        {
            addExpectTokenErrorInfo(reg->token()); // cannot be reg8
        }
        else if (reg->is(Register::IP))
        {
            if (isPush)
            { // push
                addCannotGetValueFromIPErrorInfo(e);
            }
            else
            { // pop
                addIPCannotBeModifiedErrorInfo(e);
            }
        }
        else if (reg->token().is(Token::SREG))
        {
            if (reg->is(Register::CS) && !isPush)
            {
                addCSCannotBeModifiedErrorInfo(e);
            }
            else
            {
                pp->setCode_sreg(reg->segment());
            }
        }
        else
        {
            pp->setCode_reg(reg->segment());
        }
    }
    else if (e->is(Node::ADDRESS))
    {
        if (!expectExprAbleToEvaluate(e))
        {
            return pp;
        }

        pp->setCode_rm(e->bytes());
    }
    else
    {
        addExpectExprErrorInfo(e, {Node::REGISTER, Node::ADDRESS});
    }

    return pp;
}

/* ========================================================================== */

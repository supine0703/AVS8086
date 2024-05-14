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

    auto pp =
        isPush
            ? QSharedPointer<PushPop>(new Push)
            : QSharedPointer<PushPop>(new Pop);

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        addExpectExprErrorInfo(eol);
        pp->m_expr = eol;
        return pp;
    }

    auto e = parse_expression();
    pp->m_expr = e;

    if (e->is(Node::REGISTER))
    {
        auto reg = assert_dynamic_cast<Register>(e);
        if (reg->m_token.is(Token::REG8))
        {
            addExpectTokenErrorInfo(reg->m_token); // cannot be reg8
        }
        else if (reg->m_id == Register::IP)
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
        else if (reg->m_token.is(Token::SREG))
        {
            if (reg->m_id == Register::CS && !isPush)
            {
                addCSCannotBeModifiedErrorInfo(e);
            }
            else
            {
                pp->m_codes.append(pp->SREG() | (reg->m_id & 0x18));
            }
        }
        else
        {
            pp->m_codes.append(pp->REG() | (reg->m_id & 0x07));
        }
    }
    else if (e->is(Node::ADDRESS))
    {
        if (!expectExprAbleToEvaluate(e))
        {
            return pp;
        }

        pp->m_codes.append(pp->RM());
        pp->m_codes.append(e->bytes());
        if (isPush)
        {
            pp->m_codes[1] |= 0x30;
        }
    }
    else
    {
        addExpectExprErrorInfo(e, {Node::REGISTER, Node::ADDRESS});
    }

    return pp;
}

/* ========================================================================== */

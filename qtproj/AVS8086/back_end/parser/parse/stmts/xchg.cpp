#include "parser/parser.h"
#include "ast/stmts/xchg.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_xchg()
{
    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        addExpectExprErrorInfo(eol);
        return StmtPointer(new Xchg(eol));
    }

    auto e = parse_expression();
    QSharedPointer<Xchg> xchg(new Xchg(e));

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return xchg;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->m_exprs.size() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->m_exprs.size());
        return xchg;
    }

    // XCHG e1, e2
    auto e1 = comma->m_exprs.at(0);
    auto e2 = comma->m_exprs.at(1);

    if (e1->is(Node::REGISTER))
    {
        auto reg1 = assert_dynamic_cast<Register>(e1);
        bool word = reg1->m_id & 0x08;
        if (reg1->m_token.is(Token::SREG))
        {
            addSRegCannotBeModifiedErrorInfo(e1);
            return xchg;
        }
        if (reg1->m_id == Register::IP)
        {
            addIPCannotBeModifiedErrorInfo(e1);
            return xchg;
        }

        if (e2->is(Node::REGISTER))
        {
            auto reg2 = assert_dynamic_cast<Register>(e2);
            if (reg2->m_token.is(Token::SREG))
            {
                addSRegCannotBeModifiedErrorInfo(e2);
                return xchg;
            }
            if (reg2->m_id == Register::IP)
            {
                addIPCannotBeModifiedErrorInfo(e2);
                return xchg;
            }
            if (reg1->m_token.type() != reg2->m_token.type())
            {
                addRegDoNotMatchErrorInfo(comma);
                return xchg;
            }

            if (reg1->m_id == Register::AX)
            {
                xchg->m_codes.append(Xchg::AX_REG | (reg2->m_id & 0x07));
            }
            else if (reg2->m_id == Register::AX)
            {
                xchg->m_codes.append(Xchg::AX_REG | (reg1->m_id & 0x07));
            }
            else
            {
                xchg->m_codes.append(Xchg::RM_REG | word);
                // 右边作为 r/m
                xchg->m_codes.append(
                    ((reg1->m_id & 0x07) << 3) | (reg2->m_id & 0xf7)
                );
            }
        }
        else if (e2->is(Node::ADDRESS))
        {
            if (!expectExprAbleToEvaluate(e2))
            {
                return xchg;
            }

            xchg->m_codes.append(Xchg::RM_REG | word);
            xchg->m_codes.append(e2->bytes());
            xchg->m_codes[1] |= (reg1->m_id & 0x07) << 3;
        }
        else
        {
            addExpectExprErrorInfo(e2, {Node::REGISTER, Node::ADDRESS});
        }
    }
    else if (e1->is(Node::ADDRESS))
    {
        if (!expectExprAbleToEvaluate(e1))
        {
            return xchg;
        }

        if (e2->is(Node::REGISTER))
        {
            auto reg2 = assert_dynamic_cast<Register>(e2);
            bool word = reg2->m_id & 0x08;
            xchg->m_codes.append(Xchg::RM_REG | word);
            xchg->m_codes.append(e1->bytes());
            xchg->m_codes[1] |= (reg2->m_id & 0x07) << 3;
        }
        else
        {
            addExpectExprErrorInfo(e2, {Node::REGISTER});
        }
    }
    else
    {
        addExpectExprErrorInfo(e1, {Node::REGISTER, Node::ADDRESS});
    }

    return xchg;
}

/* ========================================================================== */

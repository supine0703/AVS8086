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

    if (comma->exprCount() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
        return xchg;
    }

    // XCHG e1, e2
    auto e1 = comma->at(0);
    auto e2 = comma->at(1);

    if (e1->is(Node::REGISTER))
    { // reg, ...
        auto reg1 = assert_dynamic_cast<Register>(e1);

        if (reg1->isSegReg())
        {
            addSRegCannotBeModifiedErrorInfo(e1);
            return xchg;
        }
        if (reg1->is(Register::IP))
        {
            addIPCannotBeModifiedErrorInfo(e1);
            return xchg;
        }

        if (e2->is(Node::REGISTER))
        { // reg, reg
            auto reg2 = assert_dynamic_cast<Register>(e2);
            if (reg2->isSegReg())
            {
                addSRegCannotBeModifiedErrorInfo(e2);
                return xchg;
            }
            if (reg2->is(Register::IP))
            {
                addIPCannotBeModifiedErrorInfo(e2);
                return xchg;
            }
            if (reg1->token().type() != reg2->token().type())
            {
                addRegDoNotMatchErrorInfo(comma);
                return xchg;
            }

            // 对 AX 优化
            if (reg1->is(Register::AX))
            {  // reg, ax
                xchg->set_ax_reg(reg2->reg());
#if 0
                xchg->m_codes.append(Xchg::AX_REG | (reg2->m_id & 0x07));
#endif
            }
            else if (reg2->is(Register::AX))
            { // ax, reg
                xchg->set_ax_reg(reg1->reg());
#if 0
                xchg->m_codes.append(Xchg::AX_REG | (reg1->m_id & 0x07));
#endif
            }
            else
            { // reg, r
                xchg->set_r_reg(reg1->word(), reg2->mod_rm(), reg1->reg());
#if 0
                bool word = reg1->word();
                xchg->m_codes.append(Xchg::RM_REG | word);
                // 右边作为 r/m
                xchg->m_codes.append(
                    ((reg1->m_id & 0x07) << 3) | (reg2->m_id & 0xf7)
                );
#endif
            }
        }
        else if (e2->is(Node::ADDRESS))
        { // reg, mem
            if (!expectExprAbleToEvaluate(e2))
            {
                return xchg;
            }

            xchg->set_m_reg(reg1->word(), e2->bytes(), reg1->reg());
#if 0
            xchg->m_codes.append(Xchg::RM_REG | word);
            xchg->m_codes.append(e2->bytes());
            xchg->m_codes[1] |= (reg1->m_id & 0x07) << 3;
#endif
        }
        else
        {
            addExpectExprErrorInfo(e2, {Node::REGISTER, Node::ADDRESS});
        }
    }
    else if (e1->is(Node::ADDRESS))
    { // mem, ...
        if (!expectExprAbleToEvaluate(e1))
        {
            return xchg;
        }

        if (e2->is(Node::REGISTER))
        { // mem, reg
            auto reg2 = assert_dynamic_cast<Register>(e2);
            xchg->set_m_reg(reg2->word(), e1->bytes(), reg2->reg());
#if 0
            bool word = reg2->m_id & 0x08;
            xchg->m_codes.append(Xchg::RM_REG | word);
            xchg->m_codes.append(e1->bytes());
            xchg->m_codes[1] |= (reg2->m_id & 0x07) << 3;
#endif
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

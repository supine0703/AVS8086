#include "parser/parser.h"
#include "ast/stmts/mov.h"
#include "ast/exprs/value.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/address.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_mov()
{
    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        addExpectExprErrorInfo(eol);
        return StmtPointer(new Mov(eol));
    }

    auto e = parse_expression();
    QSharedPointer<Mov> mov(new Mov(e));

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return mov;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->exprCount() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
        return mov;
    }

    // MOV target, source
    auto target = comma->at(0);
    auto source = comma->at(1);

    // mov reg, ...
    if (target->is(Node::REGISTER))
    {
        auto tgt_reg = assert_dynamic_cast<Register>(target);

        if (tgt_reg->is(Register::CS))
        {
            addCSCannotBeModifiedErrorInfo(target);
            return mov;
        }
        if (tgt_reg->is(Register::IP))
        {
            addIPCannotBeModifiedErrorInfo(target);
            return mov;
        }

        if (source->is(Node::REGISTER))
        { // reg, reg
            auto src_reg = assert_dynamic_cast<Register>(source);
            if (src_reg->is(Register::IP))
            {
                addCannotGetValueFromIPErrorInfo(source);
                return mov;
            }
            if (tgt_reg->isSegReg() && src_reg->isSegReg())
            {
                addSRegTogeterErrorInfo(comma);
                return mov;
            }

            if (tgt_reg->isSegReg())
            {
                if (src_reg->token().is(Token::REG8))
                {
                    addRegDoNotMatchErrorInfo(comma);
                }
                else
                {
                    mov->set_r_reg(
                        Mov::SREG_RM, src_reg->mod_rm(), tgt_reg->sreg()
                    );
#if 0
                    mov->m_codes.append(Mov::SREG_RM);
                    mov->m_codes.append(
                        (src_reg->m_id & 0xf7) | (tgt_reg->m_id & 0x18)
                    );
#endif
                }
            }
            else if (src_reg->isSegReg())
            {
                if (tgt_reg->token().is(Token::REG8))
                {
                    addRegDoNotMatchErrorInfo(comma);
                }
                else
                {
                    mov->set_r_reg(
                        Mov::RM_SREG, tgt_reg->mod_rm(), src_reg->sreg()
                    );
#if 0
                    mov->m_codes.append(Mov::RM_SREG);
                    mov->m_codes.append(
                        (tgt_reg->m_id & 0xf7) | (src_reg->m_id & 0x18)
                    );
#endif
                }
            }
            else if (tgt_reg->token().type() == src_reg->token().type())
            { // 通用寄存器
                mov->set_r_reg(
                    Mov::REG_RM, src_reg->mod_rm(), tgt_reg->sreg()
                );
#if 0
                // mov->m_codes.append(Mov::RM_REG | word);
                // mov->m_codes.append(
                //     (tgt_reg->m_id & 0xf7) | ((src_reg->m_id & 0x07) << 3)
                // );
                mov->m_codes.append(Mov::REG_RM | word);
                // source 右边作为 r/m
                mov->m_codes.append(
                    ((tgt_reg->m_id & 0x07) << 3) | (src_reg->m_id & 0xf7)
                );
#endif
            }
            else
            {
                addRegDoNotMatchErrorInfo(comma);
            }
        }
        else if (source->is(Node::ADDRESS) && expectExprAbleToEvaluate(source))
        { // reg, mem
            if (!expectExprAbleToEvaluate(source))
            {
                return mov;
            }

            if (tgt_reg->isSegReg())
            {
                mov->set_rm_reg(
                    Mov::SREG_RM, source->bytes(), tgt_reg->sreg()
                );
#if 0
                mov->m_codes.append(Mov::SREG_RM);
                mov->m_codes.append(source->bytes());
                mov->m_codes[1] |= tgt_reg->m_id & 0x18;
#endif
            }
            else
            { // 通用寄存器
                auto addr = assert_dynamic_cast<Address>(source);
                if (addr->is(Address::_16))
                { // 优化 AX, AL
                    mov->set_ac_mem(source->bytes(), tgt_reg->word());
                }
                else
                {
                    mov->set_rm_reg(
                        Mov::REG_RM | tgt_reg->word(),
                        source->bytes(),
                        tgt_reg->reg()
                    );
                }
#if 0
                mov->m_codes.append(source->bytes());
                mov->m_codes[0] |= (tgt_reg->m_id & 0x07) << 3;
                if (mov->m_codes.at(0) == Address::_16)
                {
                    mov->m_codes[0] = Mov::AC_MEM | ((tgt_reg->m_id >> 3) & 1);
                }
                else
                {
                    mov->m_codes.push_front(
                        Mov::REG_RM | ((tgt_reg->m_id >> 3) & 1)
                    );
                }
#endif
            }
        }
        else if (source->is(Node::VALUE))
        { // reg, imme
            if (tgt_reg->isSegReg())
            {
                addSRegCannotBeModifiedErrorInfo(target);
                return mov;
            }

            if (!expectExprAbleToEvaluate(source))
            {
                return mov;
            }

            auto imme = assert_dynamic_cast<Value>(source);

            // 判断寄存器来进行数据对齐
            int size = 1 + tgt_reg->word();
            imme->alignData(size);
            if (imme->dataSize() != size)
            {
                addValueOverflowErrorInfo(
                    source, ((size == 1) ? 0xff : 0xffff)
                );
            }
            mov->set_reg_imme(tgt_reg->wreg(), imme->bytes());
#if 0
            mov->m_codes.append(Mov::REG_IMME | (tgt_reg->m_id & 0x0f));
            mov->m_codes.append(source->bytes());
            if (word)
            {
                if (mov->m_codes.size() == 2)
                {
                    mov->m_codes.append('\0');
                }
            }
            else if (mov->m_codes.size() != 2)
            {
                addExprVOverflowErrorInfo(source, 0xff);
            }
#endif
        }
        else
        {
            addExpectExprErrorInfo(
                source, {Node::REGISTER, Node::ADDRESS, Node::VALUE}
            );
        }
    }
    // mov mem, ...
    else if (target->is(Node::ADDRESS))
    {
        if (!expectExprAbleToEvaluate(target))
        {
            return mov;
        }

        if (source->is(Node::REGISTER))
        { // mem, reg
            auto reg = assert_dynamic_cast<Register>(source);
            if (reg->isSegReg())
            {
                mov->set_rm_reg(Mov::RM_SREG, target->bytes(), reg->sreg());
#if 0
                mov->m_codes.append(Mov::RM_SREG);
                mov->m_codes.append(target->bytes());
                mov->m_codes[1] |= reg->m_id & 0x18;
#endif
            }
            else if (reg->is(Register::IP))
            {
                addCannotGetValueFromIPErrorInfo(source);
            }
            else
            { // 通用寄存器
                auto addr = assert_dynamic_cast<Address>(target);
                if (addr->is(Address::_16))
                { // 优化 AX, AL
                    mov->set_ac_mem(target->bytes(), reg->word());
                }
                else
                {
                    mov->set_rm_reg(
                        Mov::RM_REG | reg->word(), target->bytes(), reg->reg()
                        );
                }
#if 0
                mov->m_codes.append(target->bytes());
                mov->m_codes[0] |= (reg->m_id & 0x07) << 3;
                if (mov->m_codes.at(0) == Address::_16)
                {
                    mov->m_codes[0] = Mov::MEM_AC | ((reg->m_id >> 3) & 1);
                }
                else
                {
                    mov->m_codes.push_front(
                        Mov::RM_REG | ((reg->m_id >> 3) & 1)
                    );
                }
#endif
            }
        }
        else if (source->is(Node::VALUE))
        { // mem, imme
            if (source->dataSize() > 2)
            {
                addValueOverflowErrorInfo(source, 0xffff);
            }

            mov->set_rm_imme(
                (source->dataSize() - 1), target->bytes(), source->bytes()
            );
#if 0
            mov->m_codes.append(Mov::RM_IMME | (source->dataSize() - 1));
            mov->m_codes.append(target->bytes());
            mov->m_codes.append(source->bytes());
#endif
        }
        else
        {
            addExpectExprErrorInfo(source, {Node::REGISTER, Node::VALUE});
        }
    }
    else
    {
        addExpectExprErrorInfo(target, {Node::REGISTER, Node::ADDRESS});
    }

    return mov;
}

/* ========================================================================== */

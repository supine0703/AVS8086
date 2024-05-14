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

    if (comma->m_exprs.size() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->m_exprs.size());
        return mov;
    }

    // MOV target, source
    auto target = comma->m_exprs.at(0);
    auto source = comma->m_exprs.at(1);

    if (target->is(Node::REGISTER))
    {
        auto t_reg = assert_dynamic_cast<Register>(target);

        if (t_reg->m_id == Register::CS)
        {
            addCSCannotBeModifiedErrorInfo(target);
            return mov;
        }
        if (t_reg->m_id == Register::IP)
        {
            addIPCannotBeModifiedErrorInfo(target);
            return mov;
        }

        bool word = t_reg->m_id & 0x08;

        if (source->is(Node::REGISTER))
        {
            auto s_reg = assert_dynamic_cast<Register>(source);
            if (s_reg->m_id == Register::IP)
            {
                addCannotGetValueFromIPErrorInfo(source);
                return mov;
            }
            if (t_reg->m_token.is(Token::SREG)
                && s_reg->m_token.is(Token::SREG))
            {
                addSRegTogeterErrorInfo(comma);
                return mov;
            }

            if (t_reg->m_token.is(Token::SREG))
            {
                if (s_reg->m_token.is(Token::REG8))
                {
                    addRegDoNotMatchErrorInfo(comma);
                }
                else
                {
                    mov->m_codes.append(Mov::SREG_RM);
                    mov->m_codes.append(
                        (s_reg->m_id & 0xf7) | (t_reg->m_id & 0x18)
                    );
                }
            }
            else if (s_reg->m_token.is(Token::SREG))
            {
                if (t_reg->m_token.is(Token::REG8))
                {
                    addRegDoNotMatchErrorInfo(comma);
                }
                else
                {
                    mov->m_codes.append(Mov::RM_SREG);
                    mov->m_codes.append(
                        (t_reg->m_id & 0xf7) | (s_reg->m_id & 0x18)
                    );
                }
            }
            else if (t_reg->m_token.type() == s_reg->m_token.type())
            { // 通用寄存器
#if 0
                mov->m_codes.append(Mov::RM_REG | word);
                mov->m_codes.append(
                    (t_reg->m_id & 0xf7) | ((s_reg->m_id & 0x07) << 3)
                );
#else
                mov->m_codes.append(Mov::REG_RM | word);
                // source 右边作为 r/m
                mov->m_codes.append(
                    ((t_reg->m_id & 0x07) << 3) | (s_reg->m_id & 0xf7)
                );
#endif
            }
            else
            {
                addRegDoNotMatchErrorInfo(comma);
            }
        }
        else if (source->is(Node::ADDRESS) && expectExprAbleToEvaluate(source))
        {
            if (!expectExprAbleToEvaluate(source))
            {
                return mov;
            }

            if (t_reg->m_token.is(Token::SREG))
            {
                mov->m_codes.append(Mov::SREG_RM);
                mov->m_codes.append(source->bytes());
                mov->m_codes[1] |= t_reg->m_id & 0x18;
            }
            else
            { // 通用寄存器
                mov->m_codes.append(source->bytes());
                mov->m_codes[0] |= (t_reg->m_id & 0x07) << 3;
                if (mov->m_codes.at(0) == Address::_16)
                {
                    mov->m_codes[0] = Mov::AC_MEM | ((t_reg->m_id >> 3) & 1);
                }
                else
                {
                    mov->m_codes.push_front(
                        Mov::REG_RM | ((t_reg->m_id >> 3) & 1)
                    );
                }
            }
        }
        else if (can_dynamic_cast<Value>(source))
        {
            if (target->token().is(Token::SREG))
            {
                addSRegCannotBeModifiedErrorInfo(target);
            }
            else if (expectExprAbleToEvaluate(source))
            {
                mov->m_codes.append(Mov::REG_IMME | (t_reg->m_id & 0x0f));
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
            }
        }
        else
        {
            addExpectExprErrorInfo(
                source, {Node::REGISTER, Node::ADDRESS, Node::VALUE}
            );
        }
    }
    else if (target->is(Node::ADDRESS))
    {
        if (!expectExprAbleToEvaluate(target))
        {
            return mov;
        }

        if (source->is(Node::REGISTER))
        {
            auto reg = assert_dynamic_cast<Register>(source);
            if (reg->m_token.is(Token::SREG))
            {
                mov->m_codes.append(Mov::RM_SREG);
                mov->m_codes.append(target->bytes());
                mov->m_codes[1] |= reg->m_id & 0x18;
            }
            else if (reg->m_id == Register::IP)
            {
                addCannotGetValueFromIPErrorInfo(source);
            }
            else
            {
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
            }
        }
        else if (can_dynamic_cast<Value>(source))
        {
            if (expectExprAbleToEvaluate(source))
            {
                mov->m_codes.append(Mov::RM_IMME | (source->dataSize() - 1));
                mov->m_codes.append(target->bytes());
                mov->m_codes.append(source->bytes());
            }
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

#ifndef INSTRUCTION_EXTEND_H
#define INSTRUCTION_EXTEND_H

#include "ast/stmts/instruction.h"
#include "ast/exprs/register.h"

namespace avs8086::ast {

class InstructionExtend : virtual public InstructionBase
{
public:
    virtual ~InstructionExtend() = default;

    void set_rm_reg(uint8_t code, const QByteArray& rm, uint8_t reg)
    {
        m_code.clear();
        m_code.append(rm);
        qsizetype i = Register::isSegReg(rm.at(0));
        m_code[i] |= reg << 3;
        m_code.insert(i, code);
    }

    void set_r_reg(uint8_t code, uint8_t r, uint8_t reg)
    {
        m_code.clear();
        m_code.append(code);
        m_code.append(r | (reg << 3));
    }

#if 0
    virtual void append_r_reg(uint8_t r, uint8_t reg)
    {
        m_code.append((r & 0xf7) | ((reg & 0x07) << 3));
    }
    virtual void append_rm_reg(const QByteArray& rm, uint8_t reg)
    {
        auto i = m_code.size();
        m_code.append(rm);
        m_code[i] &= 0xf7;
        m_code[i] |= (reg & 0x07) << 3;
    }
    virtual void append_r_sreg(int8_t r, int8_t sreg)
    {
        m_code.append((r & 0xf7) | (sreg & 0x18));
    }
    virtual void append_rm_sreg(const QByteArray& rm, int8_t sreg)
    {
        auto i = m_code.size();
        m_code.append(rm);
        m_code[i] &= 0xf7;
        m_code[i] |= sreg & 0x18;
    }
#endif
};

} // namespace avs8086::ast

#endif // INSTRUCTION_EXTEND_H

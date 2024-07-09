#ifndef XCHG_H
#define XCHG_H

#include "ast/stmts/instruction_extend.h"

namespace avs8086::ast {

class Xchg : public Instruction, private InstructionExtend
{
public:
    Xchg(const ExprPointer& expr) : Instruction(token::Token::XCHG, expr) { }

    ~Xchg() = default;

    void set_ax_reg(uint8_t reg)
    {
        m_code.clear();
        m_code.append(AX_REG | reg);
    }

    void set_r_reg(bool w, uint8_t r, uint8_t reg)
    { InstructionExtend::set_r_reg(RM_REG | w, r, reg); }

    void set_m_reg(bool w, const QByteArray& m, uint8_t reg)
    { set_rm_reg(RM_REG | w, m, reg); }

private:
    enum Code : uint8_t {
        // 对称的 没有左右关系
        RM_REG = 0x86,   // | w , mod reg r/m
        AX_REG = 0x90,   // | reg
    };
};


} // namespace avs8086::ast

#endif // XCHG_H

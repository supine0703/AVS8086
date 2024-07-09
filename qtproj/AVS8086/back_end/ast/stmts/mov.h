#ifndef MOV_H
#define MOV_H

#include "ast/stmts/instruction_extend.h"

namespace avs8086::ast {

class Mov : public Instruction, public InstructionExtend
{
public:
    enum Code : uint8_t {
        SREG_RM  = 0x8e,    // , mod reg r/m
        RM_SREG  = 0x8c,    // , mod reg r/m
        AC_MEM   = 0xa0,    // | w
        MEM_AC   = 0xa2,    // | w
        RM_REG   = 0x88,    // | w , mod reg r/m
        REG_RM   = 0x8a,    // | w , mod reg r/m
        RM_IMME  = 0xc6,    // | w , mod 000 r/m , imme
        REG_IMME = 0xb0,    // | wreg , imme
    };

public:
    Mov(const ExprPointer& expr)
        : Instruction(token::Token::MOV, expr)
    { }

    ~Mov() = default;

    void set_reg_imme(uint8_t wreg, const QByteArray& imme)
    {
        m_code.clear();
        m_code.append(REG_IMME | wreg);
        m_code.append(imme);
    }

    void set_rm_imme(bool w, const QByteArray& rm, const QByteArray& imme)
    {
        set_rm_reg(RM_IMME | w, rm, 0x00);
        m_code.append(imme);
    }

    void set_mem_ac(const QByteArray& mem, bool w) { set_ac(mem, MEM_AC | w); }

    void set_ac_mem(const QByteArray& mem, bool w) { set_ac(mem, AC_MEM | w); }

private:
    void set_ac(const QByteArray& mem, uint8_t code)
    {
        m_code.clear();
        m_code.append(mem);
        qsizetype i = Register::isSegReg(mem.at(0));
        m_code[i] = code;
    }


    // void append(const QByteArray& bytes) { m_code.append(bytes); }

    // void push_front(uint8_t c) { m_code.push_front(c); }

    // void replace_front(uint8_t c) { m_code[0] = c; }
};

} // namespace avs8086::ast

#endif // MOV_H

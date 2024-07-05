#ifndef PUSH_POP_H
#define PUSH_POP_H

#include "ast/stmts/instruction_extend.h"

namespace avs8086::ast {

class PushPop : public Instruction, private InstructionExtend
{
public:
    using Pointer = QSharedPointer<PushPop>;

    virtual uint8_t RM_R() const = 0;
    virtual uint8_t RM() const = 0;
    virtual uint8_t REG() const = 0;
    virtual uint8_t SREG() const = 0;

    ~PushPop() = default;

    void resetExpr(const ExprPointer& expr) { m_expr = expr; }

    void setCode_rm(const QByteArray& rm) { set_rm_reg(RM(), rm, RM_R()); }

    void setCode_reg(uint8_t reg)
    {
        m_code.clear();
        m_code.append(REG() | (reg & 0x07)); // xxxxx reg
    }

    void setCode_sreg(uint8_t sreg)
    {
        m_code.clear();
        m_code.append(SREG() | (sreg & 0x18)); // xxx sreg xxx
    }

protected:
    PushPop(token::Token::Type type)
        : Instruction(type)
    { }

    enum Code : uint8_t {
        PUSH_RM   = 0xff,   // , mod 110 r/m
        PUSH_REG  = 0x50,   // | reg
        PUSH_SREG = 0x06,   // | (reg << 3)
        POP_RM    = 0x8f,   // , mod 000 r/m
        POP_REG   = 0x58,   // | reg
        POP_SREG  = 0x07,   // | (reg << 3)
    };
};


/* ========================================================================== */

class Push : public PushPop
{
public:
    using Pointer = QSharedPointer<Push>;

    virtual uint8_t RM_R() const override { return 0x06; } // mod 110 r/m
    virtual uint8_t RM() const override { return PUSH_RM; }
    virtual uint8_t REG() const override { return PUSH_REG; }
    virtual uint8_t SREG() const override { return PUSH_SREG; }

    Push() : PushPop(token::Token::PUSH) { }

    ~Push() = default;
};

/* ========================================================================== */

class Pop : public PushPop
{
public:
    using Pointer = QSharedPointer<Pop>;

    virtual uint8_t RM_R() const override { return 0x00; } // mod 000 r/m
    virtual uint8_t RM() const override { return POP_RM; }
    virtual uint8_t REG() const override { return POP_REG; }
    virtual uint8_t SREG() const override { return POP_SREG; }

    Pop() : PushPop(token::Token::POP) { }

    ~Pop() = default;
};

} // namespace avs8086::ast

#endif // PUSH_POP_H

#ifndef SHIFT_H
#define SHIFT_H

#include "ast/stmts/instruction_extend.h"

namespace avs8086::ast {

class Shift : public Instruction, private InstructionExtend
{
public:
    using Pointer = QSharedPointer<Shift>;

    Shift(token::Token::Type type)
        : Instruction(type)
        , m_shiftReg((Q_ASSERT(sm_codes.contains(type)), sm_codes.value(type)))
    { }

    ~Shift() = default;

    void resetExpr(const ExprPointer& expr) { m_expr = expr; }

    void set_rm_one(bool word, const QByteArray& rm)
    { set_rm_reg(0xd0 | word, rm, m_shiftReg); }

    void set_rm_cl(bool word, const QByteArray& rm)
    { set_rm_reg(0xd2 | word, rm, m_shiftReg); }

private:
    enum Code : uint8_t
    {
        ROL = 0x00, // 0xd0 | vw, mod 000 r/m
        ROR = 0x01, // 0xd0 | vw, mod 001 r/m
        RCL = 0x02, // 0xd0 | vw, mod 010 r/m
        RCR = 0x03, // 0xd0 | vw, mod 011 r/m

        SHL = 0x04, // 0xd0 | vw, mod 100 r/m
        SAL = 0x04, // 0xd0 | vw, mod 100 r/m
        SHR = 0x05, // 0xd0 | vw, mod 101 r/m
        SAR = 0x07, // 0xd0 | vw, mod 111 r/m
    };

    Code m_shiftReg;

    static const QHash<token::Token::Type, Code> sm_codes;
};

} // namespace avs8086::ast

#endif // SHIFT_H

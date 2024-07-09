#ifndef LXX_H
#define LXX_H

#include "ast/stmts/instruction_extend.h"

namespace avs8086::ast {

class Lxx : public Instruction, private InstructionExtend
{
public:
    using Pointer = QSharedPointer<Lxx>;

    Lxx(token::Token::Type type)
        : Instruction(type)
        , m_lxxCode((Q_ASSERT(sm_codes.contains(type)), sm_codes.value(type)))
    { }

    ~Lxx() = default;

    void resetExpr(const ExprPointer& expr) { m_expr = expr; }

    void set_reg_imme(uint8_t reg, const QByteArray& mem)
    {
        m_code.clear();
        m_code.append(LEA_REG16_IMME | reg);
        qsizetype i = Register::isSegReg(mem.at(0)) + 1;
        m_code.append(mem.mid(i, mem.size() - i));
    }

    void set_m_reg(const QByteArray& mem, uint8_t reg)
    { set_rm_reg(m_lxxCode, mem, reg); }

    bool isLEA() const { return m_lxxCode == Code::LEA; }

private:
    enum Code : uint8_t
    {
        LEA = 0x8d, // , mod reg r/m
        LDS = 0xc5, // , mod reg r/m
        LES = 0xc4, // , mod reg r/m
        LEA_REG16_IMME = 0xb8, // | reg , imme
    };

    Code m_lxxCode;

    static const QHash<token::Token::Type, Code> sm_codes;
};

} // namespace avs8086::ast

#endif // LXX_H

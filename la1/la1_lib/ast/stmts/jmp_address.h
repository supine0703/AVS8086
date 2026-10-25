#ifndef JMP_ADDRESS_H
#define JMP_ADDRESS_H

#include "ast/stmts/instruction_extend.h"

namespace lzl::a1::ast {

class JmpAddress : public Instruction, private InstructionExtend
{ // 间接跳转
public:
    using Pointer = QSharedPointer<JmpAddress>;

    enum Code : uint8_t {
        WORD = 0x04,  // mod 100 r/m
        DWORD = 0x05, // mod 101 r/m
    };

    JmpAddress(const ExprPointer& expr) : Instruction(token::Token::JMP, expr) {}

    void setCode(const QByteArray& rm, bool d) { set_rm_reg(0xff, rm, d ? DWORD : WORD); }
};

} // namespace lzl::a1::ast

#endif // JMP_ADDRESS_H

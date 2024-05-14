#ifndef MOV_H
#define MOV_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Mov : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Mov(const ExprPointer& expr) : Statement(MOV), m_expr(expr) { }

    ~Mov() = default;

    virtual QJsonObject json() const override;

private:
    ExprPointer m_expr;

    enum Code : uint8_t {
        SREG_RM  = 0x8e,    // , mod reg r/m
        RM_SREG  = 0x8c,    // , mod reg r/m
        AC_MEM   = 0xa0,    // | w
        MEM_AC   = 0xa2,    // | w
        RM_REG   = 0x88,    // | w , mod reg r/m
        REG_RM   = 0x8a,    // | w , mod reg r/m
        RM_IMME  = 0xc6,    // | w , mod 000 r/m
        REG_IMME = 0xb0,    // | wreg
    };
};

inline QJsonObject Mov::json() const
{
    QJsonObject js = Statement::json();
    js["expr"] = m_expr->json();
    return js;
}

} // namespace avs8086::ast

#endif // MOV_H

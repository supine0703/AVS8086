#ifndef PUSH_POP_H
#define PUSH_POP_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class PushPop : public Statement
{
    friend class avs8086::parser::Parser;
public:
    virtual uint8_t RM() const = 0;
    virtual uint8_t REG() const = 0;
    virtual uint8_t SREG() const = 0;

    ~PushPop() = default;

    virtual QJsonObject json() const override;

protected:
    PushPop(Type type) : Statement(type) { }

    ExprPointer m_expr;

    enum Code : uint8_t {
        PUSH_RM   = 0xff,   // , mod 110 r/m
        PUSH_REG  = 0x50,   // | reg
        PUSH_SREG = 0x06,   // | (reg << 3)
        POP_RM    = 0x8f,   // , mod 000 r/m
        POP_REG   = 0x58,   // | reg
        POP_SREG  = 0x07,   // | (reg << 3)
    };
};

inline QJsonObject PushPop::json() const
{
    QJsonObject js = Statement::json();
    js["expr"] = m_expr->json();
    return js;
}

/* ========================================================================== */

class Push : public PushPop
{
    friend class avs8086::parser::Parser;
public:
    virtual uint8_t RM() const override { return PUSH_RM; }
    virtual uint8_t REG() const override { return PUSH_REG; }
    virtual uint8_t SREG() const override { return PUSH_SREG; }

    Push() : PushPop(PUSH) { }

    ~Push() = default;
};

/* ========================================================================== */

class Pop : public PushPop
{
    friend class avs8086::parser::Parser;
public:
    virtual uint8_t RM() const override { return POP_RM; }
    virtual uint8_t REG() const override { return POP_REG; }
    virtual uint8_t SREG() const override { return POP_SREG; }

    Pop() : PushPop(POP) { }

    ~Pop() = default;
};

} // namespace avs8086::ast

#endif // PUSH_POP_H

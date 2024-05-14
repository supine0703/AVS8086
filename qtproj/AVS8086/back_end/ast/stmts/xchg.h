#ifndef XCHG_H
#define XCHG_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Xchg : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Xchg(const ExprPointer& expr) : Statement(XCHG), m_expr(expr) { }

    ~Xchg() = default;

    virtual QJsonObject json() const override;

private:
    ExprPointer m_expr;

    enum Code : uint8_t {
        // 对称的 没有左右关系
        RM_REG = 0x86,   // | w , mod reg r/m
        AX_REG = 0x90,   // | reg
    };
};

inline QJsonObject Xchg::json() const
{
    QJsonObject js = Statement::json();
    js["expr"] = m_expr->json();
    return js;
}

} // namespace avs8086::ast

#endif // XCHG_H

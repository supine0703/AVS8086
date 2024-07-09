#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "ast/node.h"

namespace avs8086::ast {

class InstructionBase
{
public:
    virtual ~InstructionBase() = default;

protected:
    QByteArray m_code;
};

class Instruction : public Statement, virtual public InstructionBase
{
public:
    virtual ~Instruction() = default;

    virtual QJsonObject json() const override;

    virtual QByteArray code() const override { return m_code; }

    virtual qsizetype codeSize() const override { return m_code.size(); }

protected:
    Instruction(token::Token::Type type, ExprPointer expr = ExprPointer())
        : Statement(INSTRUCTION)
        , m_tokenType(type)
        , m_expr(expr)
    { }

    token::Token::Type m_tokenType;
    ExprPointer m_expr;
};

inline QJsonObject Instruction::json() const
{
    QJsonObject js = Statement::json();
    js["instruction"] = token::Token::typeName(m_tokenType);
    if (!m_expr.isNull())
    {
        js["expr"] = m_expr->json();
    }
    js["code"] = QString::fromUtf8(m_code.toHex());
    return js;
}

} // namespace avs8086::ast

#endif // INSTRUCTION_H

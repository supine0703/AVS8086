#ifndef REGISTER_H
#define REGISTER_H

#include "ast/node.h"

namespace avs8086::ast {

class RegUnion;

class Register : public Expression
{
    friend class avs8086::ast::RegUnion;
public:
    using Pointer = QSharedPointer<Register>;

    enum Id : uint8_t {
        IP = 0x0e,
        /* 访问则取 3,4 位 seg = ((Id >> 3) & 0b11) */
        ES = 0x26, // 0010 0110
        CS = 0x2e, // 0010 1110
        SS = 0x36, // 0011 0110
        DS = 0x3e, // 0011 1110
        /* mod(2b) = 11, 00, w(1b), r/m(reg)(3b)*/
        AL = 0xc0, CL, DL, BL, AH, CH, DH, BH, // 0xc0-0x77
        AX = 0xc8, CX, DX, BX, SP, BP, SI, DI, // 0xc8-0x7f
    };

public:
    Register(const token::Token& token)
        : Expression(REGISTER, token)
    {
        Q_ASSERT(sm_regId.contains((*token).toUpper()));
        m_id = sm_regId.value((*token).toUpper());
    }

    ~Register() = default;

    virtual QJsonObject json() const override;

    virtual QByteArray bytes() const override
    { return QByteArray(1, m_id); }

    virtual int dataSize() const override { return 1; }

    virtual int unitDataSize() const override { return 1; }

    bool is(Id id) const { return m_id == id; }

    bool isSegReg() const { return m_token.is(token::Token::SREG); }

    bool word() const { return !m_token.is(token::Token::REG8); }

    // 封装以下接口, 而不是暴露 m_id, 是为了通过断言防止 m_id 被滥用导致的错误
    Id segment() const { return (Q_ASSERT(isSegReg()), m_id); }

    int8_t sreg() const { return (segment() >> 3) & 0x03; }

    int8_t reg() const { return (Q_ASSERT(!isSegReg()), m_id & 0x07); }

    int8_t wreg() const { return (Q_ASSERT(!isSegReg()), m_id & 0x0f); }

    int8_t mod_rm() const { return (Q_ASSERT(!isSegReg()), m_id & 0xf7); }

    static bool isSegReg(uint8_t id) { return (id & 0xf7) == ES; }

private:
    Id m_id;

    static const QHash<QString, Id> sm_regId;
};

/* ========================================================================== */

class RegUnion : public Expression
{
public:
    using Pointer = QSharedPointer<Register>;

    RegUnion(const token::Token& token)
        : Expression(REG_UNION, token)
        , m_column(token.column())
        , m_endColumn(token.endColumn())
    { }

    ~RegUnion() = default;

    virtual QJsonObject json() const override;

    virtual Position pos() const override
    {
        return {m_token.row(), m_column, m_endColumn - m_column};
    }

    void unite(const QSharedPointer<RegUnion>& other)
    { resetPos(other); m_regs.unite(other->m_regs); }

    void insert(const QSharedPointer<Register>& reg)
    { resetPos(reg); m_regs.insert(reg->m_id, reg); }

    void resetExpr(const ExprPointer& expr)
    { resetPos(expr); m_expr = expr; }

    bool hasExpr() const { return !m_expr.isNull(); }

    ExprPointer expr() const { return m_expr; }

    QList<uint8_t> keys() const { return m_regs.keys(); }

private:
    void resetPos(const ExprPointer& e)
    {
        if (e->pos().column() < m_column)
        {
            m_column = e->pos().column();
        }
        if (m_endColumn < e->pos().endColumn())
        {
            m_endColumn = e->pos().endColumn();
        }
    }

private:
    int m_column;
    int m_endColumn;
    QMultiMap<uint8_t, QSharedPointer<Register>> m_regs;
    ExprPointer m_expr;
};

inline QJsonObject RegUnion::json() const
{
    QJsonObject js = Expression::json();
    int count = 0;
    for (const auto& reg : m_regs)
    {
        js[QString("reg %1").arg(++count)] = reg->json();
    }
    if (hasExpr())
        js["expr"] = m_expr->json();
    return js;
}

} // namespace avs8086::ast

#endif // REGISTER_H

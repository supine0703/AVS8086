#ifndef ADDRESS_H
#define ADDRESS_H

#include "ast/exprs/register.h"

namespace avs8086::parser
{
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Address : public Expression
{
    friend class avs8086::parser::Parser;
public:
    Address(const token::Token& token, const ExprPointer& expr)
        : Expression(ADDRESS, token), m_expr(expr)
    { }

    ~Address() = default;

    virtual QJsonObject json() const override;

    virtual QByteArray bytes() const override { return m_codes; }

    virtual int dataSize() const override { return m_codes.size(); }

    virtual int unitDataSize() const override { return m_codes.size(); }

    virtual Position pos() const override { return m_expr->pos(); }

private:
    bool setCode(const QList<uint8_t>& key)
    {
        auto it = sm_toCodes.find(key);
        if (it == sm_toCodes.end())
            return false;
        m_codes.append(static_cast<char>(it.value()));
        return true;
    }

    ExprPointer m_expr;
    QByteArray m_codes;

    enum Code : uint8_t {
        /* mod(2b), 00 0, r/m(3b) */
        _BX_SI    = 0x00,   _BX_DI,     _BP_SI,     _BP_DI,
        _BX_SI_8  = 0x40,   _BX_DI_8,   _BP_SI_8,   _BP_DI_8,
        _BX_SI_16 = 0x80,   _BX_DI_16,  _BP_SI_16,  _BP_DI_16,

        _SI       = 0x04,   _DI,        _16,        _BX,
        _SI_8     = 0x44,   _DI_8,      _BP_8,      _BX_8,
        _SI_16    = 0x84,   _DI_16,     _BP_16,     _BX_16
#if 0
        /* 0, r/m(3b), 00, mod(2b) */
        _BX_SI = 0x00,  _BX_SI_8,  _BX_SI_16,
        _BX_DI = 0x10,  _BX_DI_8,  _BX_DI_16,
        _BP_SI = 0x20,  _BP_SI_8,  _BP_SI_16,
        _BP_DI = 0x30,  _BP_DI_8,  _BP_DI_16,
        _SI    = 0x40,  _SI_8,     _SI_16,
        _DI    = 0x50,  _DI_8,     _DI_16,
        _16    = 0x60,  _BP_8,     _BP_16,
        _BX    = 0x70,  _BX_8,     _BX_16,
#endif
    };

    static const QHash<QList<uint8_t>, uint8_t> sm_toCodes;
};

inline QJsonObject Address::json() const
{
    QJsonObject js = Expression::json();
    js["bytes"] = QString::fromUtf8(m_codes.toHex());
    js["expr"] = m_expr->json();
    return js;
}

} // namespace avs8086::ast

#endif // ADDRESS_H

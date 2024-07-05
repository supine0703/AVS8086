#ifndef ADDRESS_H
#define ADDRESS_H

#include "ast/exprs/register.h"

namespace avs8086::ast {

class Address : public Expression
{
public:
    using Pointer = QSharedPointer<Address>;

    enum Code : uint8_t {
        /* mod(2b), 00 0, r/m(3b) */
        _BX_SI    = 0x00,   _BX_DI,     _BP_SI,     _BP_DI,
        _BX_SI_8  = 0x40,   _BX_DI_8,   _BP_SI_8,   _BP_DI_8,
        _BX_SI_16 = 0x80,   _BX_DI_16,  _BP_SI_16,  _BP_DI_16,

        _SI       = 0x04,   _DI,        _16,        _BX,
        _SI_8     = 0x44,   _DI_8,      _BP_8,      _BX_8,
        _SI_16    = 0x84,   _DI_16,     _BP_16,     _BX_16,
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

public:
    Address(const token::Token& token, const ExprPointer& expr)
        : Expression(ADDRESS, token), m_expr(expr)
    { }

    ~Address() = default;

    virtual QJsonObject json() const override;

    virtual QByteArray bytes() const override { return m_code; }

    virtual int dataSize() const override { return m_code.size(); }

    virtual int unitDataSize() const override { return m_code.size(); }

    virtual Position pos() const override { return m_expr->pos(); }

    bool is(Code code) const { return m_code[!segIsDefault()] == code; }

    bool setCode(const QList<uint8_t>& key, const QByteArray& valueCode)
    {
        auto it = sm_toCodes.find(key);
        if (it == sm_toCodes.end())
        {
            return false;
        }
        m_currSeg = m_defaultSeg =
            (key.at(0) == Register::BP) ? Register::SS : Register::DS;
        m_segReg.clear();
        m_code.clear();
        m_code.append(it.value());
        m_code.append(valueCode);
        return true;
    }

    bool segIsDefault() const { return m_currSeg == m_defaultSeg; }

    void changeSeg(const Register::Pointer& reg)
    {
        auto seg = reg->segment();
        if (m_code.isEmpty() || seg == m_currSeg)
        {
            return;
        }
        m_segReg = reg;
        if (segIsDefault())
        {
            m_code.push_front(seg);
        }
        else if (seg == m_defaultSeg)
        {
            m_code.removeFirst();
        }
        else
        {
            m_code[0] = seg;
        }
        m_currSeg = seg;
    }

private:
    Register::Pointer m_segReg;
    Register::Id m_defaultSeg;
    Register::Id m_currSeg;
    ExprPointer m_expr;
    QByteArray m_code;

    static const QHash<QList<uint8_t>, uint8_t> sm_toCodes;
};

inline QJsonObject Address::json() const
{
    QJsonObject js = Expression::json();
    js["bytes"] = QString::fromUtf8(m_code.toHex());
    js["expr"] = m_expr->json();
    if (!m_segReg.isNull())
    {
        js["segment"] = m_segReg->json();
    }
    return js;
}

} // namespace avs8086::ast

#endif // ADDRESS_H

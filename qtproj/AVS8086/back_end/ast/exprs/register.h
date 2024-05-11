#ifndef REGISTER_H
#define REGISTER_H

#include "ast/node.h"

namespace avs8086::ast {

class Register : public Expression
{
public:
    enum Id {
        IP = 0,
        /* 访问则取 3,4 位 seg = ((Id>>3)&0b11) */
        ES = 0x26, // 0010 0110
        CS = 0x2e, // 0010 1110
        SS = 0x36, // 0011 0110
        DS = 0x3e, // 0011 1110
        /* mod(2b) = 11, 00, w(1b), r/m(reg)(3b)*/
        AL = 0xc0, CL, DL, BL, AH, CH, DH, BH, // 0xc0-0xc7
        AX = 0xc8, CX, DX, BX, SP, BP, SI, DI, // 0xc8-0xcf
    };

    Register(const token::Token& token)
        : Expression(REGISTER, token)
    {
        Q_ASSERT(sm_regId.contains((*token).toUpper()));
        m_id = sm_regId.value((*token).toUpper());
    }

    virtual QJsonObject json() const override;

    token::Token::Type regType() const { return m_token.type(); }

    QString value() const { return (*m_token).toUpper(); }

private:
    Id m_id;

    static const QHash<QString, Id> sm_regId;
};

/* ========================================================================== */

class regUnion : public Expression
{

};

} // namespace avs8086::ast

#endif // REGISTER_H

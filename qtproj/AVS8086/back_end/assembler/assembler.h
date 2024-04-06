#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "token/token.h"

namespace avs8086::assembler {

class Assembler
{
public:
    Assembler();


private:
    static const QMap<token::Token::Type, QByteArray> sm_single_codes_A;
    static const QMap<QByteArray, token::Token::Type> sm_single_codes_B;
};

} // namespace avs8086::assembler

#endif // ASSEMBLER_H

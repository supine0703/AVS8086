#include "assembler/assembler.h"
#include "ast/nodes/well.hpp"

using namespace avs8086::assembler;
using namespace avs8086::token;
using namespace avs8086::ast;

void Assembler::compile_well(const QSharedPointer<Statement>& s)
{
    auto ss = qSharedPointerDynamicCast<Well>(s);
    switch (ss->valueType())
    {
    case Token::TOKEN_MAKE_:
        if (ss->value() == "BIN")
        {
            m_outType = OUT_BIN;
        }
        else if (ss->value() == "COM")
        {
            m_outType = OUT_COM;
        }
        else
        {
            m_outType = OUT_EXE;
        }
        break;
    case Token::TOKEN_LOAD_:
        if (ss->key() == "LOAD_SEGMENT")
            m_initsList[LOAD_SEGMENT] = ss->value().toInt();
        else
            m_initsList[LOAD_OFFSET] = ss->value().toInt();
        break;
    case Token::TOKEN_REGISTER:
        m_initsList[Token::findRegisters(ss->key()) - 8] =
            ss->value().toInt();
        break;
    default:
        qDebug() << "error";
    }
}



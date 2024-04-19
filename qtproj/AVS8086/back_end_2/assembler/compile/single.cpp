#include "assembler/assembler.h"
#include "ast/nodes/single.hpp"

using namespace avs8086::assembler;
using namespace avs8086::ast;

void Assembler::compile_single(const QSharedPointer<Statement>& s)
{
    auto ss = qSharedPointerDynamicCast<Single>(s);
    auto it = sm_single_codes_A.find(ss->m_token.type());
    if (it == sm_single_codes_A.end())
    {
        addErrorInfo(
            ss->m_token.row(), ss->m_token.column(),
            ss->m_token.literal().length(),
            QString("no find single instruction type: %1: %2")
                .arg(s->typeName(), ss->m_token.typeName())
        );
        return;
    }
    appendCode(it.value());
}



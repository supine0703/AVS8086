#include "ast/node.h"
#include "service/rbtset.h"

using namespace avs8086::ast;
using namespace avs8086::token;

/* ========================================================================== */

#include "ast/exprs/register.h"
const QHash<QString, Register::Id> Register::sm_regId = {
    { "ES", ES }, { "CS", CS }, { "SS", SS }, { "DS", DS },
    { "AL", AL }, { "CL", CL }, { "DL", DL }, { "BL", BL },
    { "AH", AH }, { "CH", CH }, { "DH", DH }, { "BH", BH },
    { "AX", AX }, { "CX", CX }, { "DX", DX }, { "BX", BX },
    { "SP", SP }, { "BP", BP }, { "SI", SI }, { "DI", DI },
    { "IP", IP },
};

/* ========================================================================== */

#include "ast/exprs/address.h"
const QHash<QList<uint8_t>, uint8_t> Address::sm_toCodes = {
    { { Register::BX, Register::SI, },      _BX_SI },
    { { Register::BX, Register::SI, 8 },    _BX_SI_8 },
    { { Register::BX, Register::SI, 16 },   _BX_SI_16 },
    { { Register::BX, Register::DI, },      _BX_DI },
    { { Register::BX, Register::DI, 8 },    _BX_DI_8 },
    { { Register::BX, Register::DI, 16 },   _BX_DI_16 },
    { { Register::BP, Register::SI, },      _BP_SI },
    { { Register::BP, Register::SI, 8 },    _BP_SI_8 },
    { { Register::BP, Register::SI, 16 },   _BP_SI_16 },
    { { Register::BP, Register::DI, },      _BP_DI },
    { { Register::BP, Register::DI, 8 },    _BP_DI_8 },
    { { Register::BP, Register::DI, 16 },   _BP_DI_16 },
    { { Register::SI },                     _SI },
    { { Register::SI, 8 },                  _SI_8 },
    { { Register::SI, 16 },                 _SI_16 },
    { { Register::DI },                     _DI },
    { { Register::DI, 8 },                  _DI_8 },
    { { Register::DI, 16 },                 _DI_16 },
    { { 16 },                               _16 },
    { { Register::BP, 8 },                  _BP_8 },
    { { Register::BP, 16 },                 _BP_16 },
    { { Register::BX },                     _BX },
    { { Register::BX, 8 },                  _BX_8 },
    { { Register::BX, 16 },                 _BX_16 },
};

/* ========================================================================== */

#include "ast/stmts/jmp.h"
const QHash<Token::Type, Jx::constructor> Jx::sm_constructors = {
    { Token::JO,    []() { return Pointer(new Jx(JO,   0x70)); } },
    { Token::JNO,   []() { return Pointer(new Jx(JNO,  0x71)); } },
    { Token::JB,    []() { return Pointer(new Jx(JB,   0x72)); } },
    { Token::JC,    []() { return Pointer(new Jx(JC,   0x72)); } },
    { Token::JNAE,  []() { return Pointer(new Jx(JNAE, 0x72)); } },
    { Token::JAE,   []() { return Pointer(new Jx(JAE,  0x73)); } },
    { Token::JNB,   []() { return Pointer(new Jx(JNB,  0x73)); } },
    { Token::JNC,   []() { return Pointer(new Jx(JNC,  0x73)); } },
    { Token::JE,    []() { return Pointer(new Jx(JE,   0x74)); } },
    { Token::JZ,    []() { return Pointer(new Jx(JZ,   0x74)); } },
    { Token::JNE,   []() { return Pointer(new Jx(JNE,  0x75)); } },
    { Token::JNZ,   []() { return Pointer(new Jx(JNZ,  0x75)); } },
    { Token::JBE,   []() { return Pointer(new Jx(JBE,  0x76)); } },
    { Token::JNA,   []() { return Pointer(new Jx(JNA,  0x76)); } },
    { Token::JA,    []() { return Pointer(new Jx(JA,   0x77)); } },
    { Token::JNBE,  []() { return Pointer(new Jx(JNBE, 0x77)); } },
    { Token::JS,    []() { return Pointer(new Jx(JS,   0x78)); } },
    { Token::JNS,   []() { return Pointer(new Jx(JNS,  0x79)); } },
    { Token::JP,    []() { return Pointer(new Jx(JP,   0x7a)); } },
    { Token::JPE,   []() { return Pointer(new Jx(JPE,  0x7a)); } },
    { Token::JNP,   []() { return Pointer(new Jx(JNP,  0x7b)); } },
    { Token::JPO,   []() { return Pointer(new Jx(JPO,  0x7b)); } },
    { Token::JL,    []() { return Pointer(new Jx(JL,   0x7c)); } },
    { Token::JNGE,  []() { return Pointer(new Jx(JNGE, 0x7c)); } },
    { Token::JGE,   []() { return Pointer(new Jx(JGE,  0x7d)); } },
    { Token::JNL,   []() { return Pointer(new Jx(JNL,  0x7d)); } },
    { Token::JLE,   []() { return Pointer(new Jx(JLE,  0x7e)); } },
    { Token::JNG,   []() { return Pointer(new Jx(JNG,  0x7e)); } },
    { Token::JG,    []() { return Pointer(new Jx(JG,   0x7f)); } },
    { Token::JNLE,  []() { return Pointer(new Jx(JNLE, 0x7f)); } },
    { Token::JCXZ,  []() { return Pointer(new Jx(JCXZ, 0xe3)); } },
};
#if 0
const QHash<Token::Type, char> Jx::sm_toCodes = {
    { Token::JO,        0x70 },
    { Token::JNO,       0x71 },
    { Token::JB,        0x72 },
    { Token::JC,        0x72 },
    { Token::JNAE,      0x72 },
    { Token::JAE,       0x73 },
    { Token::JNB,       0x73 },
    { Token::JNC,       0x73 },
    { Token::JE,        0x74 },
    { Token::JZ,        0x74 },
    { Token::JNE,       0x75 },
    { Token::JNZ,       0x75 },
    { Token::JBE,       0x76 },
    { Token::JNA,       0x76 },
    { Token::JA,        0x77 },
    { Token::JNBE,      0x77 },
    { Token::JS,        0x78 },
    { Token::JNS,       0x79 },
    { Token::JP,        0x7a },
    { Token::JPE,       0x7a },
    { Token::JNP,       0x7b },
    { Token::JPO,       0x7b },
    { Token::JL,        0x7c },
    { Token::JNGE,      0x7c },
    { Token::JGE,       0x7d },
    { Token::JNL,       0x7d },
    { Token::JLE,       0x7e },
    { Token::JNG,       0x7e },
    { Token::JG,        0x7f },
    { Token::JNLE,      0x7f },
    { Token::JCXZ,      0xe3 },
};
#endif

/* ========================================================================== */

#include "ast/stmts/multiple.h"
const QHash<Token::Type, Multiple::constructor> Multiple::sm_constructors = {
    { Token::LEA,   []() { return Pointer(new Multiple(LEA, 0x8d)); } },
    { Token::LDS,   []() { return Pointer(new Multiple(LDS, 0xc5)); } },
    { Token::LES,   []() { return Pointer(new Multiple(LES, 0xc4)); } },

    { Token::NOT,   []() { return Pointer(new Multiple(NOT, 0xf6)); } },
    { Token::SAL,   []() { return Pointer(new Multiple(SAL, 0xc4)); } },
    { Token::SAR,   []() { return Pointer(new Multiple(SAR, 0xc4)); } },
    { Token::SHL,   []() { return Pointer(new Multiple(SHL, 0xc4)); } },
    { Token::SHR,   []() { return Pointer(new Multiple(SHR, 0xc4)); } },
    { Token::ROL,   []() { return Pointer(new Multiple(ROL, 0xc4)); } },
    { Token::ROR,   []() { return Pointer(new Multiple(ROR, 0xc4)); } },
    { Token::RCL,   []() { return Pointer(new Multiple(RCL, 0xc4)); } },
    { Token::RCR,   []() { return Pointer(new Multiple(RCR, 0xc4)); } },
};

/* ========================================================================== */

#include "ast/stmts/single.h"
const QHash<Token::Type, Single::constructor> Single::sm_constructors = {
    { Token::XLAT,  []() { return Pointer(new Single(XLAT,  0xd7)); } },
    { Token::LAHF,  []() { return Pointer(new Single(LAHF,  0x9f)); } },
    { Token::SAHF,  []() { return Pointer(new Single(SAHF,  0x9e)); } },
    { Token::PUSHF, []() { return Pointer(new Single(PUSHF, 0x9c)); } },
    { Token::POPF,  []() { return Pointer(new Single(POPF,  0x9d)); } },
    { Token::CBW,   []() { return Pointer(new Single(CBW,   0x98)); } },
    { Token::CWD,   []() { return Pointer(new Single(CWD,   0x99)); } },
    { Token::AAA,   []() { return Pointer(new Single(AAA,   0x37)); } },
    { Token::DAA,   []() { return Pointer(new Single(DAA,   0x27)); } },
    { Token::AAS,   []() { return Pointer(new Single(AAS,   0x3f)); } },
    { Token::DAS,   []() { return Pointer(new Single(DAS,   0x2f)); } },
    { Token::INTO,  []() { return Pointer(new Single(INTO,  0xce)); } },
    { Token::IRET,  []() { return Pointer(new Single(IRET,  0xcf)); } },
    { Token::REP,   []() { return Pointer(new Single(REP,   0xf3)); } },
    { Token::REPE,  []() { return Pointer(new Single(REPE,  0xf3)); } },
    { Token::REPZ,  []() { return Pointer(new Single(REPZ,  0xf3)); } },
    { Token::REPNE, []() { return Pointer(new Single(REPNE, 0xf2)); } },
    { Token::REPNZ, []() { return Pointer(new Single(REPNZ, 0xf2)); } },
    { Token::MOVSB, []() { return Pointer(new Single(MOVSB, 0xa4)); } },
    { Token::MOVSW, []() { return Pointer(new Single(MOVSW, 0xa5)); } },
    { Token::CMPSB, []() { return Pointer(new Single(CMPSB, 0xa6)); } },
    { Token::CMPSW, []() { return Pointer(new Single(CMPSW, 0xa7)); } },
    { Token::SCASB, []() { return Pointer(new Single(SCASB, 0xae)); } },
    { Token::SCASW, []() { return Pointer(new Single(SCASW, 0xaf)); } },
    { Token::LODSB, []() { return Pointer(new Single(LODSB, 0xac)); } },
    { Token::LODSW, []() { return Pointer(new Single(LODSW, 0xad)); } },
    { Token::STOSB, []() { return Pointer(new Single(STOSB, 0xaa)); } },
    { Token::STOSW, []() { return Pointer(new Single(STOSW, 0xab)); } },
    { Token::CLC,   []() { return Pointer(new Single(CLC,   0xf8)); } },
    { Token::STC,   []() { return Pointer(new Single(STC,   0xf9)); } },
    { Token::CMC,   []() { return Pointer(new Single(CMC,   0xf5)); } },
    { Token::CLD,   []() { return Pointer(new Single(CLD,   0xfc)); } },
    { Token::STD,   []() { return Pointer(new Single(STD,   0xfd)); } },
    { Token::CLI,   []() { return Pointer(new Single(CLI,   0xfa)); } },
    { Token::STI,   []() { return Pointer(new Single(STI,   0xfb)); } },
    { Token::WAIT,  []() { return Pointer(new Single(WAIT,  0x9b)); } },
    { Token::LOCK,  []() { return Pointer(new Single(LOCK,  0xf0)); } },
    { Token::HLT,   []() { return Pointer(new Single(HLT,   0xf4)); } },
    { Token::NOP,   []() { return Pointer(new Single(NOP,   0x90)); } },

    { Token::AAM,   []() { return Pointer(new Single(AAM, {0xd4,0x0a})); } },
    { Token::AAD,   []() { return Pointer(new Single(AAD, {0xd5,0x0a})); } },
};
#if 0
const QHash<Token::Type, char> Single::sm_singleCodes = {
    { Token::XLAT,      0xd7 },     // XLAT
    { Token::LAHF,      0x9f },     // LAHF
    { Token::SAHF,      0x9e },     // SAHF
    { Token::PUSHF,     0x9c },     // PUSHF
    { Token::POPF,      0x9d },     // POPF
    { Token::CBW,       0x98 },     // CBW
    { Token::CWD,       0x99 },     // CWD
    { Token::AAA,       0x37 },     // AAA
    { Token::DAA,       0x27 },     // DAA
    { Token::AAS,       0x3f },     // AAS
    { Token::DAS,       0x2f },     // DAS
    { Token::INTO,      0xce },     // INTO
    { Token::IRET,      0xcf },     // IRET
    { Token::REP,       0xf3 },     // REP
    { Token::REPE,      0xf3 },     // REPE
    { Token::REPZ,      0xf3 },     // REPZ
    { Token::REPNE,     0xf2 },     // REPNE
    { Token::REPNZ,     0xf2 },     // REPNZ
    { Token::MOVSB,     0xa4 },     // MOVSB
    { Token::MOVSW,     0xa5 },     // MOVSW
    { Token::CMPSB,     0xa6 },     // CMPSB
    { Token::CMPSW,     0xa7 },     // CMPSW
    { Token::SCASB,     0xae },     // SCASB
    { Token::SCASW,     0xaf },     // SCASW
    { Token::LODSB,     0xac },     // LODSB
    { Token::LODSW,     0xad },     // LODSW
    { Token::STOSB,     0xaa },     // STOSB
    { Token::STOSW,     0xab },     // STOSW
    { Token::CLC,       0xf8 },     // CLC
    { Token::STC,       0xf9 },     // STC
    { Token::CMC,       0xf5 },     // CMC
    { Token::CLD,       0xfc },     // CLD
    { Token::STD,       0xfc },     // STD
    { Token::CLI,       0xfa },     // CLI
    { Token::STI,       0xfb },     // STI
    { Token::WAIT,      0x9b },     // WAIT
    { Token::LOCK,      0xf0 },     // LOCK
    { Token::HLT,       0xf4 },     // HLT
    { Token::NOP,       0x90 },     // NOP
};
const QHash<Token::Type, QPair<char, char>> Single::sm_doubleCodes = {
    { Token::AAM,       { 0xd4, 0x0a } },     // AAM
    { Token::AAD,       { 0xd5, 0x0a } },     // AAD
};
#endif

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

/* ========================================================================== */

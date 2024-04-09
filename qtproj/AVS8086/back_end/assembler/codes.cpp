#include "assembler/assembler.h"

#define BYTE_CODE QByteArray::fromRawData

using namespace avs8086::assembler;
using namespace avs8086::token;

const QMap<Token::Type, QByteArray> Assembler::sm_single_codes_A = {
    { Token::TOKEN_XLAT,        BYTE_CODE("\xd7", 1) },         // XLAT
    { Token::TOKEN_LAHF,        BYTE_CODE("\x9f", 1) },         // LAHF
    { Token::TOKEN_SAHF,        BYTE_CODE("\x9e", 1) },         // SAHF
    { Token::TOKEN_PUSHF,       BYTE_CODE("\x9c", 1) },         // PUSHF
    { Token::TOKEN_POPF,        BYTE_CODE("\x9d", 1) },         // POPF
    { Token::TOKEN_CBW,         BYTE_CODE("\x98", 1) },         // CBW
    { Token::TOKEN_CWD,         BYTE_CODE("\x99", 1) },         // CWD
    { Token::TOKEN_AAA,         BYTE_CODE("\x37", 1) },         // AAA
    { Token::TOKEN_DAA,         BYTE_CODE("\x27", 1) },         // DAA
    { Token::TOKEN_AAS,         BYTE_CODE("\x3f", 1) },         // AAS
    { Token::TOKEN_DAS,         BYTE_CODE("\x2f", 1) },         // DAS
    { Token::TOKEN_AAM,         BYTE_CODE("\xd4\x0a", 2) },     // AAM
    { Token::TOKEN_AAD,         BYTE_CODE("\xd5\x0a", 2) },     // AAD
    { Token::TOKEN_AND,         BYTE_CODE("\x24", 1) },         // AND
    { Token::TOKEN_INTO,        BYTE_CODE("\xce", 1) },         // INTO
    { Token::TOKEN_IRET,        BYTE_CODE("\xcf", 1) },         // IRET
    { Token::TOKEN_REP,         BYTE_CODE("\xf3", 1) },         // REP
    { Token::TOKEN_REPE,        BYTE_CODE("\xf3", 1) },         // REPE
    { Token::TOKEN_REPZ,        BYTE_CODE("\xf3", 1) },         // REPZ
    { Token::TOKEN_REPNE,       BYTE_CODE("\xf2", 1) },         // REPNE
    { Token::TOKEN_REPNZ,       BYTE_CODE("\xf2", 1) },         // REPNZ
    { Token::TOKEN_MOVSB,       BYTE_CODE("\xa4", 1) },         // MOVSB
    { Token::TOKEN_MOVSW,       BYTE_CODE("\xa5", 1) },         // MOVSW
    { Token::TOKEN_CMPSB,       BYTE_CODE("\xa6", 1) },         // CMPSB
    { Token::TOKEN_CMPSW,       BYTE_CODE("\xa7", 1) },         // CMPSW
    { Token::TOKEN_SCASB,       BYTE_CODE("\xae", 1) },         // SCASB
    { Token::TOKEN_SCASW,       BYTE_CODE("\xaf", 1) },         // SCASW
    { Token::TOKEN_LODSB,       BYTE_CODE("\xac", 1) },         // LODSB
    { Token::TOKEN_LODSW,       BYTE_CODE("\xad", 1) },         // LODSW
    { Token::TOKEN_STOSB,       BYTE_CODE("\xaa", 1) },         // STOSB
    { Token::TOKEN_STOSW,       BYTE_CODE("\xab", 1) },         // STOSW
    { Token::TOKEN_CLC,         BYTE_CODE("\xf8", 1) },         // CLC
    { Token::TOKEN_STC,         BYTE_CODE("\xf9", 1) },         // STC
    { Token::TOKEN_CMC,         BYTE_CODE("\xf5", 1) },         // CMC
    { Token::TOKEN_CLD,         BYTE_CODE("\xfc", 1) },         // CLD
    { Token::TOKEN_STD,         BYTE_CODE("\xfd", 1) },         // STD
    { Token::TOKEN_CLI,         BYTE_CODE("\xfa", 1) },         // CLI
    { Token::TOKEN_STI,         BYTE_CODE("\xfb", 1) },         // STI
    { Token::TOKEN_WAIT,        BYTE_CODE("\x9b", 1) },         // WAIT
    { Token::TOKEN_LOCK,        BYTE_CODE("\xf0", 1) },         // LOC
    { Token::TOKEN_HLT,         BYTE_CODE("\xf4", 1) },         // HLT
    { Token::TOKEN_NOP,         BYTE_CODE("\x90", 1) },         // NOP
};

const QMap<QByteArray, Token::Type> Assembler::sm_single_codes_B = {
    { BYTE_CODE("\xd7", 1),         Token::TOKEN_XLAT },        // XLAT
    { BYTE_CODE("\x9f", 1),         Token::TOKEN_LAHF },        // LAHF
    { BYTE_CODE("\x9e", 1),         Token::TOKEN_SAHF },        // SAHF
    { BYTE_CODE("\x9c", 1),         Token::TOKEN_PUSHF },       // PUSHF
    { BYTE_CODE("\x9d", 1),         Token::TOKEN_POPF },        // POPF
    { BYTE_CODE("\x98", 1),         Token::TOKEN_CBW },         // CBW
    { BYTE_CODE("\x99", 1),         Token::TOKEN_CWD },         // CWD
    { BYTE_CODE("\x37", 1),         Token::TOKEN_AAA },         // AAA
    { BYTE_CODE("\x27", 1),         Token::TOKEN_DAA },         // DAA
    { BYTE_CODE("\x3f", 1),         Token::TOKEN_AAS },         // AAS
    { BYTE_CODE("\x2f", 1),         Token::TOKEN_DAS },         // DAS
    { BYTE_CODE("\xd4\x0a", 2),     Token::TOKEN_AAM },         // AAM
    { BYTE_CODE("\xd5\x0a", 2),     Token::TOKEN_AAD },         // AAD
    { BYTE_CODE("\x24", 1),         Token::TOKEN_AND },         // AND
    { BYTE_CODE("\xce", 1),         Token::TOKEN_INTO },        // INTO
    { BYTE_CODE("\xcf", 1),         Token::TOKEN_IRET },        // IRET
    // { BYTE_CODE("\xf3", 1),         Token::TOKEN_REP },         // REP
    { BYTE_CODE("\xf3", 1),         Token::TOKEN_REPE },        // REPE
    // { BYTE_CODE("\xf3", 1),         Token::TOKEN_REPZ },        // REPZ
    { BYTE_CODE("\xf2", 1),         Token::TOKEN_REPNE },       // REPNE
    // { BYTE_CODE("\xf2", 1),         Token::TOKEN_REPNZ },       // REPNZ
    { BYTE_CODE("\xa4", 1),         Token::TOKEN_MOVSB },       // MOVSB
    { BYTE_CODE("\xa5", 1),         Token::TOKEN_MOVSW },       // MOVSW
    { BYTE_CODE("\xa6", 1),         Token::TOKEN_CMPSB },       // CMPSB
    { BYTE_CODE("\xa7", 1),         Token::TOKEN_CMPSW },       // CMPSW
    { BYTE_CODE("\xae", 1),         Token::TOKEN_SCASB },       // SCASB
    { BYTE_CODE("\xaf", 1),         Token::TOKEN_SCASW },       // SCASW
    { BYTE_CODE("\xac", 1),         Token::TOKEN_LODSB },       // LODSB
    { BYTE_CODE("\xad", 1),         Token::TOKEN_LODSW },       // LODSW
    { BYTE_CODE("\xaa", 1),         Token::TOKEN_STOSB },       // STOSB
    { BYTE_CODE("\xab", 1),         Token::TOKEN_STOSW },       // STOSW
    { BYTE_CODE("\xf8", 1),         Token::TOKEN_CLC },         // CLC
    { BYTE_CODE("\xf9", 1),         Token::TOKEN_STC },         // STC
    { BYTE_CODE("\xf5", 1),         Token::TOKEN_CMC },         // CMC
    { BYTE_CODE("\xfc", 1),         Token::TOKEN_CLD },         // CLD
    { BYTE_CODE("\xfd", 1),         Token::TOKEN_STD },         // STD
    { BYTE_CODE("\xfa", 1),         Token::TOKEN_CLI },         // CLI
    { BYTE_CODE("\xfb", 1),         Token::TOKEN_STI },         // STI
    { BYTE_CODE("\x9b", 1),         Token::TOKEN_WAIT },        // WAIT
    { BYTE_CODE("\xf0", 1),         Token::TOKEN_LOCK },        // LOCK
    { BYTE_CODE("\xf4", 1),         Token::TOKEN_HLT },         // HLT
    { BYTE_CODE("\x90", 1),         Token::TOKEN_NOP },         // NOP
};

const QMap<QString, QMap<Assembler::CodeBit, QString>> Assembler::sm_address_codes = {
    { "BX+SI",      { { BIT_MOD, "00" }, { BIT_R_M, "000" }, } },
    { "BX+SI+@8",   { { BIT_MOD, "01" }, { BIT_R_M, "000" }, } },
    { "BX+SI+@16",  { { BIT_MOD, "10" }, { BIT_R_M, "000" }, } },
    { "BX+DI",      { { BIT_MOD, "00" }, { BIT_R_M, "001" }, } },
    { "BX+DI+@8",   { { BIT_MOD, "01" }, { BIT_R_M, "001" }, } },
    { "BX+DI+@16",  { { BIT_MOD, "10" }, { BIT_R_M, "001" }, } },
    { "BP+SI",      { { BIT_MOD, "00" }, { BIT_R_M, "010" }, } },
    { "BP+SI+@8",   { { BIT_MOD, "01" }, { BIT_R_M, "010" }, } },
    { "BP+SI+@16",  { { BIT_MOD, "10" }, { BIT_R_M, "010" }, } },
    { "BP+DI",      { { BIT_MOD, "00" }, { BIT_R_M, "011" }, } },
    { "BP+DI+@8",   { { BIT_MOD, "01" }, { BIT_R_M, "011" }, } },
    { "BP+DI+@16",  { { BIT_MOD, "10" }, { BIT_R_M, "011" }, } },
    { "SI",         { { BIT_MOD, "00" }, { BIT_R_M, "100" }, } },
    { "SI+@8",      { { BIT_MOD, "01" }, { BIT_R_M, "100" }, } },
    { "SI+@16",     { { BIT_MOD, "10" }, { BIT_R_M, "100" }, } },
    { "DI",         { { BIT_MOD, "00" }, { BIT_R_M, "101" }, } },
    { "DI+@8",      { { BIT_MOD, "01" }, { BIT_R_M, "101" }, } },
    { "DI+@16",     { { BIT_MOD, "10" }, { BIT_R_M, "101" }, } },
    { "@16",        { { BIT_MOD, "00" }, { BIT_R_M, "110" }, } },
    { "BP+@8",      { { BIT_MOD, "01" }, { BIT_R_M, "110" }, } },
    { "BP+@16",     { { BIT_MOD, "10" }, { BIT_R_M, "110" }, } },
    { "BX",         { { BIT_MOD, "00" }, { BIT_R_M, "111" }, } },
    { "BX+@8",      { { BIT_MOD, "01" }, { BIT_R_M, "111" }, } },
    { "BX+@16",     { { BIT_MOD, "10" }, { BIT_R_M, "111" }, } },
};

const QMap<QString, QString> Assembler::sm_reg_codes = {
    { "AL" , "000" }, { "CL" , "001" }, { "DL" , "010" }, { "BL" , "011" },
    { "AH" , "100" }, { "CH" , "101" }, { "DH" , "110" }, { "BH" , "111" },
    { "AX" , "000" }, { "CX" , "001" }, { "DX" , "010" }, { "BX" , "011" },
    { "SP" , "100" }, { "BP" , "101" }, { "SI" , "110" }, { "DI" , "111" },
    { "ES" , "000" }, { "CS" , "001" }, { "SS" , "010" }, { "DS" , "011" },
};

























// XLAT  11010111
// LAHF  10011111
// SAHF  10011110
// PUSHF 10011100
// POPF  10011101
// CBW   10011000
// CWD   10011001
// AAA   00110111
// DAA   00100111
// AAS   00111111
// DAS   00101111
// AAM 11010100 000
// AAD 11010101 000
// AND   00100100
// INTO  11001110
// IRET  11001111
// REP   11110011
// REPE  11110011
// REPZ  11110011
// REPNE 11110010
// REPNZ 11110010
// MOVSB 10100100
// MOVSW 10100101
// CMPSB 10100110
// CMPSW 10100111
// SCASB 10101110
// SCASW 10101111
// LODSB 10101100
// LODSW 10101101
// STOSB 10101010
// STOSW 10101011
// CLC   11111000
// STC   11111001
// CMC   11110101
// CLD   11111100
// STD   11111101
// CLI   11111010
// STI   11111011
// WAIT  10011011
// LOCK  11110000
// HLT   11110100
// NOP   10010000

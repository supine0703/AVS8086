#ifndef TOKEN_H
#define TOKEN_H

#include <QMap>
// #include <QStringList>

namespace avs8086::token {

class Token
{
public:
    enum Type {
        TOKEN_ILLEGAL = 0,      // illegal
        TOKEN_EOF,              // eof

        TOKEN_MAKE_,            // make_
        TOKEN_LOAD_,            // load_

        TOKEN_REGISTER,         // register
        TOKEN_LABEL,            // label
        TOKEN_STRING,           // string
        TOKEN_INTEGER,          // integer
        TOKEN_FLOAT,            // float


        TOKEN_NEGATIVE,         // -x
        TOKEN_BIT_NOT,          // ~
        TOKEN_ASTERISK,         // *
        TOKEN_SLASH,            // /
        TOKEN_MODULO,           // %
        TOKEN_PLUS,             // +
        TOKEN_MINUS,            // -
        TOKEN_LEFT_SHIFT,       // <<
        TOKEN_RIGHT_SHIFT,      // >>
        TOKEN_BIT_AND,          // &
        TOKEN_BIT_XOR,          // ^
        TOKEN_BIT_OR,           // |

        TOKEN_LT,               // <
        TOKEN_GT,               // >
        TOKEN_LE,               // <=
        TOKEN_GE,               // >=
        TOKEN_EQ,               // ==
        TOKEN_NE,               // !=

        TOKEN_ASSIGN,           // =
        TOKEN_COMMA,            // ,
        TOKEN_COLON,            // :
        TOKEN_QUESTION,         // ?
        TOKEN_DOLLAR,           // $
        TOKEN_WELL,             // #
        TOKEN_ANNOTATION,       // ; annotation

        TOKEN_LPAREN,           // (
        TOKEN_RPAREN,           // )
        TOKEN_LSQUARE,          // [
        TOKEN_RSQUARE,          // ]


        TOKEN_MOV,              // MOV
        TOKEN_PUSH,             // PUSH
        TOKEN_POP,              // POP
        TOKEN_XCHG,             // XCHG
        TOKEN_XLAT,             // XLAT
        TOKEN_LEA,              // LEA
        TOKEN_LDS,              // LDS
        TOKEN_LES,              // LES

        TOKEN_LAHF,             // LAHF
        TOKEN_SAHF,             // SAHF
        TOKEN_PUSHF,            // PUSHF
        TOKEN_POPF,             // POPF

        TOKEN_IN,               // IN
        TOKEN_OUT,              // OUT

        TOKEN_ADD,              // ADD
        TOKEN_ADC,              // ADC
        TOKEN_INC,              // INC
        TOKEN_SUB,              // SUB
        TOKEN_SBB,              // SBB
        TOKEN_DEC,              // DEC
        TOKEN_NEG,              // NEG
        TOKEN_CMP,              // CMP
        TOKEN_MUL,              // MUL
        TOKEN_IMUL,             // IMUL
        TOKEN_DIV,              // DIV
        TOKEN_IDIV,             // IDIV
        TOKEN_CBW,              // CBW
        TOKEN_CWD,              // CWD
        TOKEN_AAA,              // AAA
        TOKEN_DAA,              // DAA
        TOKEN_AAS,              // AAS
        TOKEN_DAS,              // DAS
        TOKEN_AAM,              // AAM
        TOKEN_AAD,              // AAD

        TOKEN_AND,              // AND
        TOKEN_OR,               // OR
        TOKEN_XOR,              // XOR
        TOKEN_NOT,              // NOT
        TOKEN_TEST,             // TEST

        TOKEN_SAL,              // SAL
        TOKEN_SAR,              // SAR
        TOKEN_SHL,              // SHL
        TOKEN_SHR,              // SHR
        TOKEN_ROL,              // ROL
        TOKEN_ROR,              // ROR
        TOKEN_RCL,              // RCL
        TOKEN_RCR,              // RCR

        TOKEN_MOVS,             // MOVS
        TOKEN_MOVSB,            // MOVSB
        TOKEN_MOVSW,            // MOVSW
        TOKEN_CMPS,             // CMPS
        TOKEN_CMPSB,            // CMPSB
        TOKEN_CMPSW,            // CMPSW
        TOKEN_SCAS,             // SCAS
        TOKEN_SCASB,            // SCASB
        TOKEN_SCASW,            // SCASW
        TOKEN_LODS,             // LODS
        TOKEN_LODSB,            // LODSB
        TOKEN_LODSW,            // LODSW
        TOKEN_STOS,             // STOS
        TOKEN_STOSB,            // STOSB
        TOKEN_STOSW,            // STOSW

        TOKEN_REP,              // REP
        TOKEN_REPE,             // REPE
        TOKEN_REPZ,             // REPZ
        TOKEN_REPNE,            // REPNE
        TOKEN_REPNZ,            // REPNZ

        TOKEN_CALL,             // CALL
        TOKEN_RET,              // RET
        TOKEN_JMP,              // any
        TOKEN_JA,               // CF == 0 && ZF == 0  unsigned >
        TOKEN_JNBE,             // CF == 0 && ZF == 0
        TOKEN_JAE,              // CF == 0  unsigned >=
        TOKEN_JNB,              // CF == 0
        TOKEN_JB,               // CF == 1  unsigned <
        TOKEN_JNAE,             // CF == 1
        TOKEN_JBE,              // CF == 1 || ZF == 1  unsigned <=
        TOKEN_JNA,              // CF == 1 || ZF == 1
        TOKEN_JC,               // CF == 1  if carry
        TOKEN_JNC,              // CF == 0  if !carry
        TOKEN_JE,               // ZF == 1  if == 0
        TOKEN_JZ,               // ZF == 1
        TOKEN_JNE,              // ZF == 0  if != 0
        TOKEN_JNZ,              // ZF == 0
        TOKEN_JG,               // ZF == 0 && SF == OF  signed >
        TOKEN_JNLE,             // ZF == 0 && SF == OF
        TOKEN_JGE,              // SF == OF  signed >=
        TOKEN_JNL,              // SF == OF
        TOKEN_JL,               // SF != OF  signed <
        TOKEN_JNGE,             // SF != OF
        TOKEN_JLE,              // ZF == 1 || SF != OF  signed <=
        TOKEN_JNG,              // ZF == 1 || SF != OF
        TOKEN_JO,               // OF == 1  if overflow
        TOKEN_JNO,              // OF == 0  if !overflow
        TOKEN_JNP,              // PF == 0  if % 2 == 0
        TOKEN_JPO,              // PF == 0
        TOKEN_JP,               // PF == 1  if % 2 == 1
        TOKEN_JPE,              // PF == 1
        TOKEN_JNS,              // SF == 0  if >= 0
        TOKEN_JS,               // SF == 1  if <  0
        TOKEN_LOOP,
        TOKEN_LOOPE,            // != 0
        TOKEN_LOOPZ,            // != 0
        TOKEN_LOOPNE,           // != 0
        TOKEN_LOOPNZ,           // != 0
        TOKEN_JCXZ,             // CX == 0

        TOKEN_INT,              // INT
        TOKEN_INTO,             // INTO
        TOKEN_IRET,             // IRET

        TOKEN_CLC,              // CLC
        TOKEN_STC,              // STC
        TOKEN_CMC,              // CMC
        TOKEN_CLD,              // CLD
        TOKEN_STD,              // STD
        TOKEN_CLI,              // CLI
        TOKEN_STI,              // STI

        TOKEN_WAIT,             // WAIT
        TOKEN_ESC,              // ESC
        TOKEN_LOCK,             // LOCK

        TOKEN_HLT,              // HLT
        TOKEN_NOP,              // NOP


        TOKEN_INCLUDE,          // INCLUDE
        TOKEN_ORG,              // ORG
        TOKEN_EQU,              // EQU
        TOKEN_DB,               // DB
        TOKEN_DW,               // DW
        TOKEN_DD,               // DD
        TOKEN_DQ,               // DQ
        TOKEN_DT,               // DT
    };

public:
    Token();
    Token(Type type, const QString& literal, int row, int column);
    Token(const Token& other);
    ~Token();

    Token& operator=(const Token& other);

    bool is(Type type) const;
    Type type() const;
    QString literal() const;
    int row() const;
    int column() const;

    void resetType(Type type);

    QString typeName() const;
    static QString tokenTypeName(Type type);
    static Type tokenType(const QChar& literal);
    static Type tokenType(const QChar& l1, const QChar& l2);
    static Type tokenType(const QString& literal);

    static int textToInt(const QString& numStr);
    static int lastTextToInt();

    static bool textIsFloat(const QString& numStr);

private:
    Type m_type;
    QString m_literal;
    int m_row;
    int m_column;

    static int sm_lastTextToInt;

    static const QMap<Type, QString> sm_typeNames;

    static const QStringList sm_registers;
    static const QMap<Type, QString> sm_mnemonics_A;
    static const QMap<QString, Type> sm_mnemonics_B;

    static const QMap<Type, QString> sm_symbols_A;
    static const QMap<QString, Type> sm_symbols_B;
    static const QMap<QString, Type> sm_symbols_C;

    static const QStringList sm_makeNames;
    static const QStringList sm_loadNames;
};

} // namespace avs8086::token

#endif // TOKEN_H

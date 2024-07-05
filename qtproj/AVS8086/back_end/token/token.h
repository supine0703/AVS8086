#ifndef TOKEN_H
#define TOKEN_H

#include "service/position.h"

namespace avs8086::token {

class Token
{
public:
    enum Type  {
        TOKEN_EOL = -2,     // eol: end of line
        TOKEN_EOF = -1,     // eof: end of file

        ILLEGAL = 0,        // illegal
        ILLEGAL_INTEGER,    // illegal integer

        MAKE_X,             // make_
        LOAD_X,             // load_

        // REGISTER,           // register
        SREG,               // segment register
        REG8,               // register 8bit
        REG16,              // register 16bit

        IDENTIFIER,         // identifier
        STRING,             // string
        INTEGER,            // integer
        FLOAT,              // float

        LINE_BREAK,         // \ line break
        ANNOTATION,         // ; annotation

        BIT_NOT,            // ~
        ASTERISK,           // *
        SLASH,              // /
        MODULO,             // %
        PLUS,               // +
        MINUS,              // -
        LEFT_SHIFT,         // <<
        RIGHT_SHIFT,        // >>
        BIT_AND,            // &
        BIT_XOR,            // ^
        BIT_OR,             // |

        LT,                 // <
        GT,                 // >
        LE,                 // <=
        GE,                 // >=
        EQ,                 // ==
        NE,                 // !=

        ASSIGN,             // =
        COMMA,              // ,
        COLON,              // :
        QUESTION,           // ?
        DOLLAR,             // $
        WELL,               // #

        LPAREN,             // (
        RPAREN,             // )
        LSQUARE,            // [
        RSQUARE,            // ]


        MOV,                // MOV
        PUSH,               // PUSH
        POP,                // POP
        XCHG,               // XCHG
        XLAT,               // XLAT
        LEA,                // LEA
        LDS,                // LDS
        LES,                // LES

        LAHF,               // LAHF
        SAHF,               // SAHF
        PUSHF,              // PUSHF
        POPF,               // POPF

        IN,                 // IN
        OUT,                // OUT

        ADD,                // ADD
        ADC,                // ADC
        INC,                // INC
        SUB,                // SUB
        SBB,                // SBB
        DEC,                // DEC
        NEG,                // NEG
        CMP,                // CMP
        MUL,                // MUL
        IMUL,               // IMUL
        DIV,                // DIV
        IDIV,               // IDIV
        CBW,                // CBW
        CWD,                // CWD
        AAA,                // AAA
        DAA,                // DAA
        AAS,                // AAS
        DAS,                // DAS
        AAM,                // AAM
        AAD,                // AAD

        AND,                // AND
        OR,                 // OR
        XOR,                // XOR
        NOT,                // NOT
        TEST,               // TEST

        SAL,                // SAL
        SAR,                // SAR
        SHL,                // SHL
        SHR,                // SHR
        ROL,                // ROL
        ROR,                // ROR
        RCL,                // RCL
        RCR,                // RCR

        MOVSB,              // MOVSB
        MOVSW,              // MOVSW
        CMPSB,              // CMPSB
        CMPSW,              // CMPSW
        SCASB,              // SCASB
        SCASW,              // SCASW
        LODSB,              // LODSB
        LODSW,              // LODSW
        STOSB,              // STOSB
        STOSW,              // STOSW

        REP,                // REP
        REPE,               // REPE
        REPZ,               // REPZ
        REPNE,              // REPNE
        REPNZ,              // REPNZ

        CALL,               // CALL
        RET,                // RET
        JMP,                // any
        JA,                 // CF == 0 && ZF == 0  unsigned >
        JNBE,               // CF == 0 && ZF == 0
        JAE,                // CF == 0  unsigned >=
        JNB,                // CF == 0
        JB,                 // CF == 1  unsigned <
        JNAE,               // CF == 1
        JBE,                // CF == 1 || ZF == 1  unsigned <=
        JNA,                // CF == 1 || ZF == 1
        JC,                 // CF == 1  if carry
        JNC,                // CF == 0  if !carry
        JE,                 // ZF == 1  if == 0
        JZ,                 // ZF == 1
        JNE,                // ZF == 0  if != 0
        JNZ,                // ZF == 0
        JG,                 // ZF == 0 && SF == OF  signed >
        JNLE,               // ZF == 0 && SF == OF
        JGE,                // SF == OF  signed >=
        JNL,                // SF == OF
        JL,                 // SF != OF  signed <
        JNGE,               // SF != OF
        JLE,                // ZF == 1 || SF != OF  signed <=
        JNG,                // ZF == 1 || SF != OF
        JO,                 // OF == 1  if overflow
        JNO,                // OF == 0  if !overflow
        JNP,                // PF == 0  if % 2 == 0
        JPO,                // PF == 0
        JP,                 // PF == 1  if % 2 == 1
        JPE,                // PF == 1
        JNS,                // SF == 0  if >= 0
        JS,                 // SF == 1  if <  0
        LOOP,
        LOOPE,              // == 0
        LOOPZ,              // == 0
        LOOPNE,             // != 0
        LOOPNZ,             // != 0
        JCXZ,               // CX == 0

        INT,                // INT
        INTO,               // INTO
        IRET,               // IRET

        CLC,                // CLC
        STC,                // STC
        CMC,                // CMC
        CLD,                // CLD
        STD,                // STD
        CLI,                // CLI
        STI,                // STI

        WAIT,               // WAIT
        ESC,                // ESC
        LOCK,               // LOCK

        HLT,                // HLT
        NOP,                // NOP


        INCLUDE,            // INCLUDE
        ALLOCATE,           // DB DW DD DQ DT
        ORG,                // ORG
        EQU,                // EQU  
        DUP,

        PTR,
        SHORT,
        NEAR,
        FAR,
        BYTE,
        WORD,
        DWORD,

        SEGMENT,
        ENDS,
        END,

        PROC,
        PROC_NEAR,
        PROC_FAR,
    };

public:
    Token() : m_type(ILLEGAL) { }

    Token(Type type, const QString& literal, int row, int column)
        : m_type(type)
        , m_literal(literal)
        , m_pos(row, column, literal.length())
    { }

    ~Token() = default;

    void resetType(Type type) { m_type = type; }

    bool is(Type type) const { return m_type == type; }

    Type type() const { return m_type; }

    QString literal() const { return m_literal; }

    Position pos() const { return m_pos; }

    Position::Value posValue() const { return m_pos.value(); }

    int row() const { return m_pos.row(); }

    int column() const { return m_pos.column(); }

    int length() const { return m_pos.length(); }

    int endColumn() const { return m_pos.endColumn(); }

    QString typeName() const { return typeName(m_type); }

    QString operator*() const { return m_literal; }

    QString content() const;

    static QString typeName(Type type);
    static Type type(const QChar& literal);
    static Type type(const QChar& literal1, const QChar& literal2);
    static Type type(const QString& literal);

    static bool textIsFloat(const QString& numStr);

    // 0: 不是数字, -1: 数字不合法, 1: 可;
    static int textIsInteger(const QString& numStr, size_t* num = nullptr);

    size_t toInt(bool* ok = nullptr) const
    {
        size_t n = 0;
        if (ok != nullptr)
            *ok = (textIsInteger(m_literal, &n) == 1);
        else
            textIsInteger(m_literal, &n);
        return n;
    }


private:
    Type m_type;
    QString m_literal;
    Position m_pos;

#if 0
    /**
     * @m_row, @m_column
     * QPlainTextEdit 的行列号返回 int, 所以这里存储用 int
    */
    int m_row;
    int m_column;
#endif

    static const QString sm_illegalName;
    static const QHash<Type, QString> sm_contents;
    static const QHash<Type, QString> sm_typeNames;
    static const QHash<QString, Type> sm_singleSymbols;
    static const QHash<QString, Type> sm_doubleSymbols;
    static const QHash<QString, Type> sm_mnemonics;

#if 0
    // 多对一 放在 set+hash 中
    static const QSet<QString> sm_makes;
    static const QSet<QString> sm_loads;
    static const QSet<QString> sm_sregs;
    static const QSet<QString> sm_reg8s;
    static const QSet<QString> sm_reg16s;
    static const QSet<QString> sm_defs;
    static const QHash<const QSet<QString>*, Type> sm_setTypes;
#endif
};

using TokenList = QList<Token>;

} // namespace avs8086::token

#endif // TOKEN_H

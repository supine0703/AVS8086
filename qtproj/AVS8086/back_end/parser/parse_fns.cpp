#include "parser.h"
#include "ast/expr_headers.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

const QHash<Token::Type, Parser::stmt_parseFn> Parser::sm_stmt_parseFns = {
    { Token::WELL,          &Parser::parse_well },          // #...#
    { Token::ALLOCATE,      &Parser::parse_allocate },      // allocate

    { Token::MOV,           &Parser::parse_mov },           // MOV
    { Token::PUSH,          &Parser::parse_push_pop },      // PUSH
    { Token::POP,           &Parser::parse_push_pop },      // POP
    { Token::XCHG,          &Parser::parse_xchg },          // XCHG
    { Token::LEA,           &Parser::parse_lxx },           // LEA
    { Token::LDS,           &Parser::parse_lxx },           // LDS
    { Token::LES,           &Parser::parse_lxx },           // LES

    // { Token::IN,            &Parser::parse_reserved_word }, // IN
    // { Token::OUT,           &Parser::parse_reserved_word }, // OUT

    { Token::NOT,           &Parser::parse_logical_not },   // NOT
    { Token::AND,           &Parser::parse_logical_bit },   // AND
    { Token::OR,            &Parser::parse_logical_bit },   // OR
    { Token::XOR,           &Parser::parse_logical_bit },   // XOR
    { Token::TEST,          &Parser::parse_logical_bit },   // TEST
    { Token::SAL,           &Parser::parse_logical_shift }, // SAL
    { Token::SAR,           &Parser::parse_logical_shift }, // SAR
    { Token::SHL,           &Parser::parse_logical_shift }, // SHL
    { Token::SHR,           &Parser::parse_logical_shift }, // SHR
    { Token::ROL,           &Parser::parse_logical_shift }, // ROL
    { Token::ROR,           &Parser::parse_logical_shift }, // ROR
    { Token::RCL,           &Parser::parse_logical_shift }, // RCL
    { Token::RCR,           &Parser::parse_logical_shift }, // RCR

    { Token::JMP,           &Parser::parse_jmp },           // JMP
    { Token::JNBE,          &Parser::parse_jx },            // JNBE
    { Token::JA,            &Parser::parse_jx },            // JA
    { Token::JAE,           &Parser::parse_jx },            // JAE
    { Token::JNB,           &Parser::parse_jx },            // JNB
    { Token::JB,            &Parser::parse_jx },            // JB
    { Token::JNAE,          &Parser::parse_jx },            // JNAE
    { Token::JBE,           &Parser::parse_jx },            // JBE
    { Token::JNA,           &Parser::parse_jx },            // JNA
    { Token::JC,            &Parser::parse_jx },            // JC
    { Token::JNC,           &Parser::parse_jx },            // JNC
    { Token::JE,            &Parser::parse_jx },            // JE
    { Token::JZ,            &Parser::parse_jx },            // JZ
    { Token::JNE,           &Parser::parse_jx },            // JNE
    { Token::JNZ,           &Parser::parse_jx },            // JNZ
    { Token::JG,            &Parser::parse_jx },            // JG
    { Token::JNLE,          &Parser::parse_jx },            // JNLE
    { Token::JGE,           &Parser::parse_jx },            // JGE
    { Token::JNL,           &Parser::parse_jx },            // JNL
    { Token::JL,            &Parser::parse_jx },            // JL
    { Token::JNGE,          &Parser::parse_jx },            // JNGE
    { Token::JLE,           &Parser::parse_jx },            // JLE
    { Token::JNG,           &Parser::parse_jx },            // JNG
    { Token::JO,            &Parser::parse_jx },            // JO
    { Token::JNO,           &Parser::parse_jx },            // JNO
    { Token::JNP,           &Parser::parse_jx },            // JNP
    { Token::JPO,           &Parser::parse_jx },            // JPO
    { Token::JP,            &Parser::parse_jx },            // JP
    { Token::JPE,           &Parser::parse_jx },            // JPE
    { Token::JNS,           &Parser::parse_jx },            // JNS
    { Token::JS,            &Parser::parse_jx },            // JS
    { Token::JCXZ,          &Parser::parse_jx },            // JCXZ

    { Token::XLAT,          &Parser::parse_single },        // XLAT
    { Token::LAHF,          &Parser::parse_single },        // LAHF
    { Token::SAHF,          &Parser::parse_single },        // SAHF
    { Token::PUSHF,         &Parser::parse_single },        // PUSHF
    { Token::POPF,          &Parser::parse_single },        // POPF
    { Token::CBW,           &Parser::parse_single },        // CBW
    { Token::CWD,           &Parser::parse_single },        // CWD
    { Token::AAA,           &Parser::parse_single },        // AAA
    { Token::DAA,           &Parser::parse_single },        // DAA
    { Token::AAS,           &Parser::parse_single },        // AAS
    { Token::DAS,           &Parser::parse_single },        // DAS
    { Token::AAM,           &Parser::parse_single },        // AAM
    { Token::AAD,           &Parser::parse_single },        // AAD
    { Token::INTO,          &Parser::parse_single },        // INTO
    { Token::IRET,          &Parser::parse_single },        // IRET
    { Token::REP,           &Parser::parse_single },        // REP
    { Token::REPE,          &Parser::parse_single },        // REPE
    { Token::REPZ,          &Parser::parse_single },        // REPZ
    { Token::REPNE,         &Parser::parse_single },        // REPNE
    { Token::REPNZ,         &Parser::parse_single },        // REPNZ
    { Token::MOVSB,         &Parser::parse_single },        // MOVSB
    { Token::MOVSW,         &Parser::parse_single },        // MOVSW
    { Token::CMPSB,         &Parser::parse_single },        // CMPSB
    { Token::CMPSW,         &Parser::parse_single },        // CMPSW
    { Token::SCASB,         &Parser::parse_single },        // SCASB
    { Token::SCASW,         &Parser::parse_single },        // SCASW
    { Token::LODSB,         &Parser::parse_single },        // LODSB
    { Token::LODSW,         &Parser::parse_single },        // LODSW
    { Token::STOSB,         &Parser::parse_single },        // STOSB
    { Token::STOSW,         &Parser::parse_single },        // STOSW
    { Token::CLC,           &Parser::parse_single },        // CLC
    { Token::STC,           &Parser::parse_single },        // STC
    { Token::CMC,           &Parser::parse_single },        // CMC
    { Token::CLD,           &Parser::parse_single },        // CLD
    { Token::STD,           &Parser::parse_single },        // STD
    { Token::CLI,           &Parser::parse_single },        // CLI
    { Token::STI,           &Parser::parse_single },        // STI
    { Token::WAIT,          &Parser::parse_single },        // WAIT
    { Token::LOCK,          &Parser::parse_single },        // LOCK
    { Token::HLT,           &Parser::parse_single },        // HLT
    { Token::NOP,           &Parser::parse_single },        // NOP
};

/* ========================================================================== */

const QHash<Token::Type, Parser::post_parseFn> Parser::sm_post_parseFns = {
    { Token::COLON,         &Parser::parse_define },
    { Token::SEGMENT,       &Parser::parse_define },
    { Token::ALLOCATE,      &Parser::parse_define },
};

/* ========================================================================== */

const QHash<Token::Type, Parser::prefix_parseFn> Parser::sm_prefix_parseFns = {
    { Token::ILLEGAL,       &Parser::parse_T<Illegal> },    // illegal

    { Token::INTEGER,       &Parser::parse_value },         // integer
    { Token::STRING,        &Parser::parse_value },         // string
    { Token::FLOAT,         &Parser::parse_value },         // float

    { Token::SREG,          &Parser::parse_T<Register> },   // sreg
    { Token::REG8,          &Parser::parse_T<Register> },   // reg8
    { Token::REG16,         &Parser::parse_T<Register> },   // reg16
    { Token::MAKE_X,        &Parser::parse_T<Make_X> },     // make_x
    { Token::LOAD_X,        &Parser::parse_T<Load_X> },     // load_x
    { Token::BIT_NOT,       &Parser::parse_operator },      // ~x
    { Token::PLUS,          &Parser::parse_operator },      // +x
    { Token::MINUS,         &Parser::parse_operator },      // -x

    { Token::LPAREN,        &Parser::parse_group },         // (
    { Token::LSQUARE,       &Parser::parse_address },       // [

    // { Token::TOKEN_EOF,   &Parser::parse_not_end },       // eof
    // { Token::ANNOTATION,  &Parser::parse_not_end },       // ;
    // { Token::BIT_NOT,     &Parser::parse_prefix },        // ~x
    // { Token::PLUS,        &Parser::parse_prefix },        // +x
    // { Token::MINUS,       &Parser::parse_prefix },        // -x
    // { Token::FLOAT,       &Parser::parse_float },         // float
    // { Token::INTEGER,     &Parser::parse_integer },       // integer
    // { Token::STRING,      &Parser::parse_string },        // string

    // { Token::REGISTER,    &Parser::parse_register },      // reg
    // { Token::LSQUARE,     &Parser::parse_address },       // [

    // { Token::LOCK,        &Parser::parse_reserved_word }, // lock
};

/* ========================================================================== */

const QHash<Token::Type, Parser::infix_parseFn> Parser::sm_infix_parseFns = {
    { Token::ASSIGN,        &Parser::parse_T<Assign> },     // =
    { Token::COMMA,         &Parser::parse_T<Comma> },      // ,
    { Token::COLON,         &Parser::parse_T<Colon> },      // :
    { Token::DUP,           &Parser::parse_dup },           // dup

    { Token::ASTERISK,        &Parser::parse_operator },    // *
    { Token::SLASH,           &Parser::parse_operator },    // /
    { Token::MODULO,          &Parser::parse_operator },    // %
    { Token::PLUS,            &Parser::parse_operator },    // +
    { Token::MINUS,           &Parser::parse_operator },    // -
    { Token::LEFT_SHIFT,      &Parser::parse_operator },    // <<
    { Token::RIGHT_SHIFT,     &Parser::parse_operator },    // >>
    { Token::BIT_AND,         &Parser::parse_operator },    // &
    { Token::BIT_XOR,         &Parser::parse_operator },    // ^
    { Token::BIT_OR,          &Parser::parse_operator },    // |
    { Token::LT,              &Parser::parse_operator },    // <
    { Token::GT,              &Parser::parse_operator },    // >
    { Token::LE,              &Parser::parse_operator },    // <=
    { Token::GE,              &Parser::parse_operator },    // >=
    { Token::EQ,              &Parser::parse_operator },    // ==
    { Token::NE,              &Parser::parse_operator },    // !=

    // { Token::COMMA,           &Parser::parse_comma },     // ,
    // { Token::COLON,           &Parser::parse_colon },     // :
};

/* ========================================================================== */

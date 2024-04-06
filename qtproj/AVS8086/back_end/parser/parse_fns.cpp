#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::parser;

const QMap<Token::Type, Parser::stmt_parse_fn> Parser::sm_stmt_parse_fns = {
    { Token::TOKEN_WELL,        &Parser::parse_well },      // #
    { Token::TOKEN_MOV,         &Parser::parse_mov },       // mov

    { Token::TOKEN_XLAT,        &Parser::parse_single },    // XLAT
    { Token::TOKEN_LAHF,        &Parser::parse_single },    // LAHF
    { Token::TOKEN_SAHF,        &Parser::parse_single },    // SAHF
    { Token::TOKEN_PUSHF,       &Parser::parse_single },    // PUSHF
    { Token::TOKEN_POPF,        &Parser::parse_single },    // POPF
    { Token::TOKEN_CBW,         &Parser::parse_single },    // CBW
    { Token::TOKEN_CWD,         &Parser::parse_single },    // CWD
    { Token::TOKEN_AAA,         &Parser::parse_single },    // AAA
    { Token::TOKEN_DAA,         &Parser::parse_single },    // DAA
    { Token::TOKEN_AAS,         &Parser::parse_single },    // AAS
    { Token::TOKEN_DAS,         &Parser::parse_single },    // DAS
    { Token::TOKEN_AAM,         &Parser::parse_single },    // AAM
    { Token::TOKEN_AAD,         &Parser::parse_single },    // AAD
    { Token::TOKEN_AND,         &Parser::parse_single },    // AND
    { Token::TOKEN_INTO,        &Parser::parse_single },    // INTO
    { Token::TOKEN_IRET,        &Parser::parse_single },    // IRET
    { Token::TOKEN_REP,         &Parser::parse_single },    // REP
    { Token::TOKEN_REPE,        &Parser::parse_single },    // REPE
    { Token::TOKEN_REPZ,        &Parser::parse_single },    // REPZ
    { Token::TOKEN_REPNE,       &Parser::parse_single },    // REPNE
    { Token::TOKEN_REPNZ,       &Parser::parse_single },    // REPNZ
    { Token::TOKEN_MOVSB,       &Parser::parse_single },    // MOVSB
    { Token::TOKEN_MOVSW,       &Parser::parse_single },    // MOVSW
    { Token::TOKEN_CMPSB,       &Parser::parse_single },    // CMPSB
    { Token::TOKEN_CMPSW,       &Parser::parse_single },    // CMPSW
    { Token::TOKEN_SCASB,       &Parser::parse_single },    // SCASB
    { Token::TOKEN_SCASW,       &Parser::parse_single },    // SCASW
    { Token::TOKEN_LODSB,       &Parser::parse_single },    // LODSB
    { Token::TOKEN_LODSW,       &Parser::parse_single },    // LODSW
    { Token::TOKEN_STOSB,       &Parser::parse_single },    // STOSB
    { Token::TOKEN_STOSW,       &Parser::parse_single },    // STOSW
    { Token::TOKEN_CLC,         &Parser::parse_single },    // CLC
    { Token::TOKEN_STC,         &Parser::parse_single },    // STC
    { Token::TOKEN_CMC,         &Parser::parse_single },    // CMC
    { Token::TOKEN_CLD,         &Parser::parse_single },    // CLD
    { Token::TOKEN_STD,         &Parser::parse_single },    // STD
    { Token::TOKEN_CLI,         &Parser::parse_single },    // CLI
    { Token::TOKEN_STI,         &Parser::parse_single },    // STI
    { Token::TOKEN_WAIT,        &Parser::parse_single },    // WAIT
    { Token::TOKEN_LOCK,        &Parser::parse_single },    // LOCK
    { Token::TOKEN_HLT,         &Parser::parse_single },    // HLT
    { Token::TOKEN_NOP,         &Parser::parse_single },    // NOP

};

const QMap<Token::Type, Parser::prefix_parse_fn> Parser::sm_prefix_parse_fns = {
    { Token::TOKEN_ILLEGAL,     &Parser::parse_illegal },       // illegal
    { Token::TOKEN_EOF,         &Parser::parse_not_end },       // eof
    { Token::TOKEN_ANNOTATION,  &Parser::parse_not_end },       // ;
    { Token::TOKEN_BIT_NOT,     &Parser::parse_prefix },        // ~x
    { Token::TOKEN_PLUS,        &Parser::parse_prefix },        // +x
    { Token::TOKEN_MINUS,       &Parser::parse_prefix },        // -x
    { Token::TOKEN_FLOAT,       &Parser::parse_float },         // float
    { Token::TOKEN_INTEGER,     &Parser::parse_integer },       // integer
    { Token::TOKEN_STRING,      &Parser::parse_string },        // string
    { Token::TOKEN_LPAREN,      &Parser::parse_group },         // (

    { Token::TOKEN_LSQUARE,     &Parser::parse_address },       // [
    { Token::TOKEN_LOCK,        &Parser::parse_reserved_word }, // lock
};

const QMap<Token::Type, Parser::infix_parse_fn> Parser::sm_infix_parse_fns = {
    { Token::TOKEN_BIT_NOT,         &Parser::parse_infix },     // ~
    { Token::TOKEN_ASTERISK,        &Parser::parse_infix },     // *
    { Token::TOKEN_SLASH,           &Parser::parse_infix },     // /
    { Token::TOKEN_MODULO,          &Parser::parse_infix },     // %
    { Token::TOKEN_PLUS,            &Parser::parse_infix },     // +
    { Token::TOKEN_MINUS,           &Parser::parse_infix },     // -
    { Token::TOKEN_LEFT_SHIFT,      &Parser::parse_infix },     // <<
    { Token::TOKEN_RIGHT_SHIFT,     &Parser::parse_infix },     // >>
    { Token::TOKEN_BIT_AND,         &Parser::parse_infix },     // &
    { Token::TOKEN_BIT_XOR,         &Parser::parse_infix },     // ^
    { Token::TOKEN_BIT_OR,          &Parser::parse_infix },     // |
    { Token::TOKEN_LT,              &Parser::parse_infix },     // <
    { Token::TOKEN_GT,              &Parser::parse_infix },     // >
    { Token::TOKEN_LE,              &Parser::parse_infix },     // <=
    { Token::TOKEN_GE,              &Parser::parse_infix },     // >=
    { Token::TOKEN_EQ,              &Parser::parse_infix },     // ==
    { Token::TOKEN_NE,              &Parser::parse_infix },     // !=

    { Token::TOKEN_COMMA,           &Parser::parse_comma },     // ,

    // { Token::TOKEN_COLON,           &Parser::parse_colon },     // :
};


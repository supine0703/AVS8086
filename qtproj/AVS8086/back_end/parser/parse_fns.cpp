#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::parser;

const QHash<Token::Type, Parser::stmt_parseFn> Parser::sm_stmt_parseFns = {
    { Token::WELL,          &Parser::parse_well },          // #...#
    { Token::IDENTIFIER,    &Parser::parse_identifier },    // identifier
    { Token::JMP,           &Parser::parse_jmp },           // jmp
    // { Token::MOV,         &Parser::parse_mov },       // mov
    // { Token::ADD,         &Parser::parse_mov },       // add
    // { Token::ADC,         &Parser::parse_mov },       // adc

    // { Token::XLAT,        &Parser::parse_single },    // XLAT
    // { Token::LAHF,        &Parser::parse_single },    // LAHF
    // { Token::SAHF,        &Parser::parse_single },    // SAHF
    // { Token::PUSHF,       &Parser::parse_single },    // PUSHF
    // { Token::POPF,        &Parser::parse_single },    // POPF
    // { Token::CBW,         &Parser::parse_single },    // CBW
    // { Token::CWD,         &Parser::parse_single },    // CWD
    // { Token::AAA,         &Parser::parse_single },    // AAA
    // { Token::DAA,         &Parser::parse_single },    // DAA
    // { Token::AAS,         &Parser::parse_single },    // AAS
    // { Token::DAS,         &Parser::parse_single },    // DAS
    // { Token::AAM,         &Parser::parse_single },    // AAM
    // { Token::AAD,         &Parser::parse_single },    // AAD
    // { Token::AND,         &Parser::parse_single },    // AND
    // { Token::INTO,        &Parser::parse_single },    // INTO
    // { Token::IRET,        &Parser::parse_single },    // IRET
    // { Token::REP,         &Parser::parse_single },    // REP
    // { Token::REPE,        &Parser::parse_single },    // REPE
    // { Token::REPZ,        &Parser::parse_single },    // REPZ
    // { Token::REPNE,       &Parser::parse_single },    // REPNE
    // { Token::REPNZ,       &Parser::parse_single },    // REPNZ
    // { Token::MOVSB,       &Parser::parse_single },    // MOVSB
    // { Token::MOVSW,       &Parser::parse_single },    // MOVSW
    // { Token::CMPSB,       &Parser::parse_single },    // CMPSB
    // { Token::CMPSW,       &Parser::parse_single },    // CMPSW
    // { Token::SCASB,       &Parser::parse_single },    // SCASB
    // { Token::SCASW,       &Parser::parse_single },    // SCASW
    // { Token::LODSB,       &Parser::parse_single },    // LODSB
    // { Token::LODSW,       &Parser::parse_single },    // LODSW
    // { Token::STOSB,       &Parser::parse_single },    // STOSB
    // { Token::STOSW,       &Parser::parse_single },    // STOSW
    // { Token::CLC,         &Parser::parse_single },    // CLC
    // { Token::STC,         &Parser::parse_single },    // STC
    // { Token::CMC,         &Parser::parse_single },    // CMC
    // { Token::CLD,         &Parser::parse_single },    // CLD
    // { Token::STD,         &Parser::parse_single },    // STD
    // { Token::CLI,         &Parser::parse_single },    // CLI
    // { Token::STI,         &Parser::parse_single },    // STI
    // { Token::WAIT,        &Parser::parse_single },    // WAIT
    // { Token::LOCK,        &Parser::parse_single },    // LOCK
    // { Token::HLT,         &Parser::parse_single },    // HLT
    // { Token::NOP,         &Parser::parse_single },    // NOP

};

const QHash<Token::Type, Parser::prefix_parseFn> Parser::sm_prefix_parseFns = {
    { Token::ILLEGAL,     &Parser::parse_illegal },       // illegal
    // { Token::TOKEN_EOF,   &Parser::parse_not_end },       // eof
    // { Token::ANNOTATION,  &Parser::parse_not_end },       // ;
    // { Token::BIT_NOT,     &Parser::parse_prefix },        // ~x
    // { Token::PLUS,        &Parser::parse_prefix },        // +x
    // { Token::MINUS,       &Parser::parse_prefix },        // -x
    // { Token::FLOAT,       &Parser::parse_float },         // float
    // { Token::INTEGER,     &Parser::parse_integer },       // integer
    // { Token::STRING,      &Parser::parse_string },        // string
    // { Token::LPAREN,      &Parser::parse_group },         // (

    // { Token::REGISTER,    &Parser::parse_register },      // reg
    // { Token::LSQUARE,     &Parser::parse_address },       // [

    // { Token::LOCK,        &Parser::parse_reserved_word }, // lock
};

const QHash<Token::Type, Parser::infix_parseFn> Parser::sm_infix_parseFns = {
    // { Token::BIT_NOT,         &Parser::parse_infix },     // ~
    // { Token::ASTERISK,        &Parser::parse_infix },     // *
    // { Token::SLASH,           &Parser::parse_infix },     // /
    // { Token::MODULO,          &Parser::parse_infix },     // %
    // { Token::PLUS,            &Parser::parse_infix },     // +
    // { Token::MINUS,           &Parser::parse_infix },     // -
    // { Token::LEFT_SHIFT,      &Parser::parse_infix },     // <<
    // { Token::RIGHT_SHIFT,     &Parser::parse_infix },     // >>
    // { Token::BIT_AND,         &Parser::parse_infix },     // &
    // { Token::BIT_XOR,         &Parser::parse_infix },     // ^
    // { Token::BIT_OR,          &Parser::parse_infix },     // |
    // { Token::LT,              &Parser::parse_infix },     // <
    // { Token::GT,              &Parser::parse_infix },     // >
    // { Token::LE,              &Parser::parse_infix },     // <=
    // { Token::GE,              &Parser::parse_infix },     // >=
    // { Token::EQ,              &Parser::parse_infix },     // ==
    // { Token::NE,              &Parser::parse_infix },     // !=

    // { Token::COMMA,           &Parser::parse_comma },     // ,
    // { Token::COLON,           &Parser::parse_colon },     // :
};


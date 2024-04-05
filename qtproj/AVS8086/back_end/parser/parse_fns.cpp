#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::parser;

const QMap<Token::Type, Parser::stmt_parse_fn> Parser::sm_stmt_parse_fns = {
    { Token::TOKEN_WELL,        &Parser::parse_well },      // #
    { Token::TOKEN_MOV,         &Parser::parse_mov },       // mov

};

const QMap<Token::Type, Parser::prefix_parse_fn> Parser::sm_prefix_parse_fns = {
    { Token::TOKEN_ILLEGAL,     &Parser::parse_illegal },       // illegal
    { Token::TOKEN_EOF,         &Parser::parse_not_end },       // eof
    { Token::TOKEN_ANNOTATION,  &Parser::parse_not_end },       // ;
    { Token::TOKEN_BIT_NOT,     &Parser::parse_prefix },        // ~x
    { Token::TOKEN_PLUS,        &Parser::parse_prefix },        // +x
    { Token::TOKEN_MINUS,       &Parser::parse_prefix },        // -x
    { Token::TOKEN_FLOAT,       &Parser::parse_float },         // integer
    { Token::TOKEN_INTEGER,     &Parser::parse_integer },       // integer
    { Token::TOKEN_STRING,      &Parser::parse_string },        // string
    { Token::TOKEN_LPAREN,      &Parser::parse_group },         // (

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


#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::parser;

const QMap<Token::Type, Parser::stmt_parseFn> Parser::sm_statement_parseFns = {
    { Token::TOKEN_MOV,         &Parser::parse_mov },       // mov
};

const QMap<Token::Type, Parser::prefix_parseFn> Parser::sm_prefix_parseFns = {
    { Token::TOKEN_EOF,         &Parser::parse_not_end },       // eof
    // { Token::TOKEN_ANNOTATION,  &Parser::parse_not_end },       // ;
    { Token::TOKEN_MINUS,       &Parser::parse_nagative },      // -x
    { Token::TOKEN_INTEGER,     &Parser::parse_integer },       // integer
    { Token::TOKEN_STRING,      &Parser::parse_string },        // integer
    { Token::TOKEN_LPAREN,      &Parser::parse_group },         // (
};

const QMap<Token::Type, Parser::infix_parseFn> Parser::sm_infix_parseFns = {
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
};

const QMap<Token::Type, Parser::postfix_parseFn> Parser::sm_postfix_parseFns = {
    { Token::TOKEN_COLON,       nullptr },
};

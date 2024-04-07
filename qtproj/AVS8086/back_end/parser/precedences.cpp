#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::parser;

const QMap<Token::Type, Parser::Precedence> Parser::sm_precedences = {
    { Token::TOKEN_ASTERISK,        PRODUCT },      // *
    { Token::TOKEN_SLASH,           PRODUCT },      // /
    { Token::TOKEN_MODULO,          PRODUCT },      // %
    { Token::TOKEN_PLUS,            SUM },          // +
    { Token::TOKEN_MINUS,           SUM },          // -
    { Token::TOKEN_LEFT_SHIFT,      SHIFT },        // <<
    { Token::TOKEN_RIGHT_SHIFT,     SHIFT },        // >>
    { Token::TOKEN_LT,              COMPARE },      // <
    { Token::TOKEN_GT,              COMPARE },      // >
    { Token::TOKEN_LE,              COMPARE },      // <=
    { Token::TOKEN_GE,              COMPARE },      // >=
    { Token::TOKEN_EQ,              COMPARE },      // ==
    { Token::TOKEN_NE,              COMPARE },      // !=
    { Token::TOKEN_BIT_AND,         LOGIC },        // &
    { Token::TOKEN_BIT_XOR,         LOGIC },        // ^
    { Token::TOKEN_BIT_OR,          LOGIC },        // |
    { Token::TOKEN_COMMA,           COMMA },        // ,
    { Token::TOKEN_COLON,           ASSIGN },       // :
    { Token::TOKEN_LSQUARE,         ADDRESS },      // []
};


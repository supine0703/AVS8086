#include "parser/parser.h"

using namespace avs8086::token;
using namespace avs8086::parser;

const QMap<Token::Type, Parser::Precedence> Parser::sm_precedences = {
    { Token::ASTERISK,        PRODUCT },      // *
    { Token::SLASH,           PRODUCT },      // /
    { Token::MODULO,          PRODUCT },      // %
    { Token::PLUS,            SUM },          // +
    { Token::MINUS,           SUM },          // -
    { Token::LEFT_SHIFT,      SHIFT },        // <<
    { Token::RIGHT_SHIFT,     SHIFT },        // >>
    { Token::LT,              COMPARE },      // <
    { Token::GT,              COMPARE },      // >
    { Token::LE,              COMPARE },      // <=
    { Token::GE,              COMPARE },      // >=
    { Token::EQ,              COMPARE },      // ==
    { Token::NE,              COMPARE },      // !=
    { Token::BIT_AND,         LOGIC },        // &
    { Token::BIT_XOR,         LOGIC },        // ^
    { Token::BIT_OR,          LOGIC },        // |
    { Token::COMMA,           COMMA },        // ,
    { Token::COLON,           ASSIGN },       // :
    { Token::LSQUARE,         ADDRESS },      // []
};


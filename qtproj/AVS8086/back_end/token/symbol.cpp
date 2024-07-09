#include "token.h"

using namespace avs8086::token;

const QHash<QString, Token::Type> Token::sm_singleSymbols = {
    { "\n",             TOKEN_EOL },
    { "",               TOKEN_EOF },
    { ";",              ANNOTATION },
    { "~",              BIT_NOT },
    { "*",              ASTERISK },
    { "/",              SLASH },
    { "%",              MODULO },
    { "+",              PLUS },
    { "-",              MINUS },
    { "&",              BIT_AND },
    { "^",              BIT_XOR },
    { "|",              BIT_OR },
    { "<",              LT },
    { ">",              GT },
    { "=",              ASSIGN },
    { ",",              COMMA },
    { ":",              COLON },
    { "?",              QUESTION },
    { "$",              DOLLAR },
    { "#",              WELL },
    { "(",              LPAREN },
    { ")",              RPAREN },
    { "[",              LSQUARE },
    { "]",              RSQUARE },
    { "\\",             LINE_BREAK },
};

const QHash<QString, Token::Type> Token::sm_doubleSymbols = {
    { "<<",             LEFT_SHIFT },
    { ">>",             RIGHT_SHIFT },
    { "<=",             LE },
    { ">=",             GE },
    { "==",             EQ },
    { "!=",             NE },
};

#include "token/token.h"

using namespace avs8086::token;

const QMap<Token::Type, QString> Token::sm_symbols_A = {
    { TOKEN_EOF,                "EOF" },
    { TOKEN_ANNOTATION,         ";Annotation" },
    { TOKEN_BIT_NOT,            "~" },
    { TOKEN_ASTERISK,           "*" },
    { TOKEN_SLASH,              "/" },
    { TOKEN_MODULO,             "%" },
    { TOKEN_PLUS,               "+" },
    { TOKEN_MINUS,              "-" },
    { TOKEN_LEFT_SHIFT,         "<<" },
    { TOKEN_RIGHT_SHIFT,        ">>" },
    { TOKEN_BIT_AND,            "&" },
    { TOKEN_BIT_XOR,            "^" },
    { TOKEN_BIT_OR,             "|" },
    { TOKEN_LT,                 "<" },
    { TOKEN_GT,                 ">" },
    { TOKEN_LE,                 "<=" },
    { TOKEN_GE,                 ">=" },
    { TOKEN_EQ,                 "==" },
    { TOKEN_NE,                 "!=" },
    { TOKEN_ASSIGN,             "=" },
    { TOKEN_COMMA,              "," },
    { TOKEN_COLON,              ":" },
    { TOKEN_QUESTION,           "?" },
    { TOKEN_DOLLAR,             "$" },
    { TOKEN_WELL,               "#" },
    { TOKEN_LPAREN,             "(" },
    { TOKEN_RPAREN,             ")" },
    { TOKEN_LSQUARE,            "[" },
    { TOKEN_RSQUARE,            "]" },
};

const QMap<QString, Token::Type> Token::sm_symbols_B = {
    { "",               TOKEN_EOF },
    { ";",              TOKEN_ANNOTATION },
    { "~",              TOKEN_BIT_NOT },
    { "*",              TOKEN_ASTERISK },
    { "/",              TOKEN_SLASH },
    { "%",              TOKEN_MODULO },
    { "+",              TOKEN_PLUS },
    { "-",              TOKEN_MINUS },
    { "&",              TOKEN_BIT_AND },
    { "^",              TOKEN_BIT_XOR },
    { "|",              TOKEN_BIT_OR },
    { "<",              TOKEN_LT },
    { ">",              TOKEN_GT },
    { "=",              TOKEN_ASSIGN },
    { ",",              TOKEN_COMMA },
    { ":",              TOKEN_COLON },
    { "?",              TOKEN_QUESTION },
    { "$",              TOKEN_DOLLAR },
    { "#",              TOKEN_WELL },
    { "(",              TOKEN_LPAREN },
    { ")",              TOKEN_RPAREN },
    { "[",              TOKEN_LSQUARE },
    { "]",              TOKEN_RSQUARE },
};

const QMap<QString, Token::Type> Token::sm_symbols_C = {
    { "<<",             TOKEN_LEFT_SHIFT },
    { ">>",             TOKEN_RIGHT_SHIFT },
    { "<=",             TOKEN_LE },
    { ">=",             TOKEN_GE },
    { "==",             TOKEN_EQ },
    { "!=",             TOKEN_NE },
};

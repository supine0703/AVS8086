#include "token.h"

using namespace lzl;
using namespace lzl::a1::token;

const HashMap<QString, Token::Type> Token::s_singleSymbols = {
    {"\n", TOKEN_EOL },
    {"",   TOKEN_EOF },
    {";",  ANNOTATION},
    {"~",  BIT_NOT   },
    {"*",  ASTERISK  },
    {"/",  SLASH     },
    {"%",  MODULO    },
    {"+",  PLUS      },
    {"-",  MINUS     },
    {"&",  BIT_AND   },
    {"^",  BIT_XOR   },
    {"|",  BIT_OR    },
    {"<",  LT        },
    {">",  GT        },
    {"=",  ASSIGN    },
    {",",  COMMA     },
    {":",  COLON     },
    {"?",  QUESTION  },
    {"$",  DOLLAR    },
    {"#",  WELL      },
    {"(",  LPAREN    },
    {")",  RPAREN    },
    {"[",  LSQUARE   },
    {"]",  RSQUARE   },
    {"\\", LINE_BREAK},
};

const HashMap<QString, Token::Type> Token::s_doubleSymbols = {
    {"<<", LEFT_SHIFT },
    {">>", RIGHT_SHIFT},
    {"<=", LE         },
    {">=", GE         },
    {"==", EQ         },
    {"!=", NE         },
};

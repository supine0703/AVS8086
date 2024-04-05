#include "token/token.h"

using namespace avs8086::token;

const QMap<Token::Type, QString> Token::sm_typeNames = {
    { TOKEN_ILLEGAL,        "Illegal" },
    { TOKEN_LABEL,          "Label"},
    { TOKEN_STRING,         "String"},
    { TOKEN_INTEGER,        "Integer" },
    { TOKEN_FLOAT,          "Float" },
    { TOKEN_MAKE_,          "MAKE_" },
    { TOKEN_LOAD_,          "LOAD_" },
};

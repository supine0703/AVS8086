#include "token/token.h"

using namespace avs8086::token;

const QStringList Token::sm_makeNames = {
    "MAKE_BIN", "MAKE_COM", "MAKE_EXE",
};

const QStringList Token::sm_loadNames = {
    "LOAD_SEGMENT", "LOAD_OFFSET",
};

const QMap<Token::Type, QString> Token::sm_typeNames = {
    { ILLEGAL,            "Illegal" },
    { LABEL,              "Label"},
    { REGISTER,           "Register" },
    { STRING,             "String"},
    { INTEGER,            "Integer" },
    { FLOAT,              "Float" },
    { MAKE_X,             "MAKE_X" },
    { LOAD_X,             "LOAD_X" },
};

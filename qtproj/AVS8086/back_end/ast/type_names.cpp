#include "node.h"

using namespace avs8086::ast;

const QMap<Node::Type, QString> Node::sm_typeNames = {
    { ILLEGAL,                 "Illegal" },
    { INTEGER,                 "Integer" },
    { STRING,                  "String" },

    { PREFIX,                  "Prefix" },
    { INFIX,                   "Infix" },

    { COMMA,                   "',' comma"},
    { COLON,                   "':'label or reg"},

    { LABEL,                   "Label" },
    { REGISTER,                "Register" },
    { ADDRESS,                 "[Address]"},

    { PROGRAM,                 "Program" },
    { EXPRESSION_STATEMENT,    "Expression Statement" },
    { MULTIPLE_STATEMENT,      "Multiple statement" },


    { SINGLE,                  "Single Instruction" },
    { WELL,                    "'#' Instruction" },
    { MOV,                     "MOV" },
};

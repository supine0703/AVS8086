#include "node.h"

using namespace avs8086::ast;

const QHash<Node::Type, QString> Node::sm_typeNames = {
    { INTEGER,                 "Integer" },
    // { STRING,                  "String" },

    // { PREFIX,                  "Prefix" },
    // { INFIX,                   "Infix" },

    // { COMMA,                   "',' comma"},
    // { COLON,                   "':'label or reg"},

    // { LABEL,                   "Label" },
    // { REGISTER,                "Register" },
    // { ADDRESS,                 "[Address]"},

    // { PROGRAM,                 "Program" },
    // { EXPRESSION_STATEMENT,    "Expression Statement" },
    // { MULTIPLE_STATEMENT,      "Multiple statement" },

    // { WELL,                    "'#' Instruction" },
    // { SINGLE,                  "Single Instruction" },
    // { MOV,                     "MOV" },
};

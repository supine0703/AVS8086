#include "node.h"

using namespace avs8086::ast;

const QMap<Node::Type, QString> Node::sm_typeNames = {
    { NODE_ILLEGAL,                 "Illegal" },
    { NODE_INTEGER,                 "Integer" },
    { NODE_STRING,                  "String" },

    { NODE_PREFIX,                  "Prefix" },
    { NODE_INFIX,                   "Infix" },

    { NODE_COMMA,                   "',' comma"},
    { NODE_COLON,                   "':'label or reg"},

    { NODE_LABEL,                   "Label" },
    { NODE_REGISTER,                "Register" },
    { NODE_ADDRESS,                 "[Address]"},

    { NODE_PROGRAM,                 "Program" },
    { NODE_EXPRESSION_STATEMENT,    "Expression Statement" },
    { NODE_MULTIPLE_STATEMENT,      "Multiple statement" },


    { NODE_SINGLE,                  "Single Instruction" },
    { NODE_WELL,                    "'#' Instruction" },
    { NODE_MOV,                     "MOV" },
};

#include "node.h"

using namespace avs8086::ast;

const QMap<Node::Type, QString> Node::sm_typeNames = {
    { NODE_ILLEGAL,                 "Illegal" },
    { NODE_PROGRAM,                 "Program" },
    { NODE_EXPRESSION_STATEMENT,    "Expression Statement" },
    { NODE_INFIX,                   "Infix" },
    { NODE_STRING,                  "String" },
    { NODE_INTEGER,                 "Integer" },
    { NODE_PREFIX,                  "Prefix" },

    { NODE_REGISTER,                "Register" },
    { NODE_SINGLE,                  "Single Instruction" },
    { NODE_WELL,                    "'#' Instruction" },
    { NODE_MOV,                     "MOV" },
};

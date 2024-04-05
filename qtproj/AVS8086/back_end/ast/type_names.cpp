#include "node.h"

using namespace avs8086::ast;

const QMap<Node::Type, QString> Node::sm_typeNames = {
    { NODE_ILLEGAL,                 "Illegal" },
    { NODE_PROGRAM,                 "Program" },
    { NODE_EXPRESSION_STATEMENT,    "ExpressionStatement" },
    { NODE_INFIX,                   "Infix" },
    { NODE_STRING,                  "String" },
    { NODE_INTEGER,                 "Integer" },
    { NODE_PREFIX,                  "Prefix" },

    { NODE_REGISTER,                "Register" },
    { NODE_WELL,                    "#" },
    { NODE_MOV,                     "MOV" },
};

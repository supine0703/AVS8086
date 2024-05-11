#include "node.h"

using namespace avs8086::ast;

const QHash<Node::Type, QString> Node::sm_typeNames = {
    { NODE_EOL,         "EOL" },

    // value
    { DUP,              "DUP"   },
    { VALUE,            "Value" },
    { FLOAT,            "Float" },
    { OPERATOR,         "Operator" },
    { IDENTIFIER,       "Identifier" },

    { PROGRAM,          "Program" },
    { WELL,             "'#'Instruction" },
    { SINGLE,           "Single Instruction" },
    { DEFINE,           "Define" },
    { JMP,              "JMP" },
    { JX,               "JX" },
    { MOV,              "MOV" },

    // expression
    { COMMA,            "Comma" },
    { COMMA_ARRAY,      "Comma Array" },

    { REGISTER,         "Register" },

    { MAKE_X,           "MAKE_X" },
    { ASSIGN,           "Assign" },

    { EXPR_STMT,        "Expression Statement" },
};

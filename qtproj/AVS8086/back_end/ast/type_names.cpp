#include "node.h"

using namespace avs8086::ast;

/* ========================================================================== */

const QHash<Node::Type, QString> Node::sm_typeNames = {
    { NODE_EOL,         "EOL" },

    // value
    { DUP,              "DUP" },
    { VALUE,            "Value" },
    { FLOAT,            "Float" },
    { OPERATOR,         "Operator" },
    { IDENTIFIER,       "Identifier" },

    { PROGRAM,          "Program" },
    { DEFINE,           "Define" },
    { WELL,             "'#'Instruction" },

    // expression
    { COMMA,            "Comma" },
    { COMMA_ARRAY,      "Comma Array" },

    { REGISTER,         "Register" },
    { REG_UNION,        "Register Union" },
    { ADDRESS,          "Address" },

    { MAKE_X,           "MAKE_X" },
    { ASSIGN,           "Assign" },

    { EXPR_STMT,        "Expression Statement" },

    { INSTRUCTION,      "Instruction" },

    { SINGLE,           "Single Instruction" },

};

/* ========================================================================== */

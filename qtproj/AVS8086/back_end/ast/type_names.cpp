#include "node.h"

using namespace avs8086::ast;

const QHash<Node::Type, QString> Node::sm_typeNames = {
    { NODE_EOL,                 "EOL" },

    // value
    { DUP,                      "DUP"   },
    { VALUE,                    "Value" },
    { FLOAT,                    "Float" },
    { OPERATOR,                 "Operator" },
    { IDENTIFIER,               "Identifier" },

    { PROGRAM,                  "Program" },
    { WELL,                     "'#'Instruction" },
    { DEFINE,                   "Define" },

    // expression
    { COMMA,                    "Comma" },
    { COMMA_ARRAY,              "Comma Array" },

    { REGISTER,                 "Register" },

    { MAKE_X,                   "MAKE_X" },
    { ASSIGN,                   "Assign" },

    { INFIX,                    "Infix" },



    // { PREFIX,                  "Prefix" },


    // { COMMA,                   "',' comma"},
    // { COLON,                   "':'label or reg"},

    // { LABEL,                   "Label" },

    // { ADDRESS,                 "[Address]"},


    // { EXPRESSION_STATEMENT,    "Expression Statement" },
    // { MULTIPLE_STATEMENT,      "Multiple statement" },


    // { SINGLE,                  "Single Instruction" },
    // { MOV,                     "MOV" },
};

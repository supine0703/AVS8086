#include "assembler/assembler.h"

using namespace avs8086::ast;
using namespace avs8086::assembler;

const QMap<Node::Type, Assembler::compile_fn>Assembler::sm_compile_fns = {
    { Node::NODE_SINGLE,        &Assembler::compile_single },
    { Node::NODE_MOV,           &Assembler::compile_mov },
};


#include "node.h"

using namespace avs8086::ast;

/* ========================================================================== */

QString Node::typeName(Type type)
{
    auto it = sm_typeNames.find(type);
    if (it != sm_typeNames.end())
        return it.value();
    return "Illegal";
}

/* ========================================================================== */


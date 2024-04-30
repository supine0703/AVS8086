#include "node.h"

using namespace avs8086::ast;

Node::Node(Type type)
    : m_type(type)
{ }

Node::~Node()
{ }

/* ========================================================================== */

#if 0
bool Node::isError() const
{
    return m_isError;
}

void Node::goError()
{
    m_isError = true;
}
#endif

bool Node::is(Type type) const
{
    return m_type == type;
}

Node::Type Node::type() const
{
    return m_type;
}

QString Node::typeName() const
{
    return typeName(m_type);
}

/* ========================================================================== */

QString Node::typeName(Type type)
{
    auto it = sm_typeNames.find(type);
    if (it != sm_typeNames.end())
        return it.value();
    return "Illegal";
}

/* ========================================================================== */


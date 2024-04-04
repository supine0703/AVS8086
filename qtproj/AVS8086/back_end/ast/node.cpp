#include "node.h"

using namespace avs8086::ast;

Node::Node(Type type)
    : m_type(type), m_isError(false)
{ }

Node::~Node()
{ }

/* ========================================================================== */

bool Node::isError() const
{
    return m_isError;
}

void Node::goError()
{
    m_isError = true;
}

Node::Type Node::type() const
{
    return m_type;
}

QString Node::typeName() const
{
    return nodeTypeName(m_type);
}

/* ========================================================================== */

QString Node::nodeTypeName(Type type)
{
    auto it = sm_typeNames.find(type);
    return it != sm_typeNames.end() ? it.value() : "";
}


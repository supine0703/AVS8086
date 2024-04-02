#include "node.h"


Node::Node(Type type)
    : m_type(type)
{ }

Node::~Node()
{ }

/* ========================================================================== */

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
    return sm_names.contains(type) ? sm_names.value(type) : "";
}

/* ========================================================================== */

const QMap<Node::Type, QString> Node::sm_names = {
    { NODE_ILLEGAL,                 "Illegal" },
    { NODE_PROGRAM,                 "Program" },
    { NODE_EXPRESSION_STATEMENT,    "ExpressionStatement" },
    { NODE_PREFIX,                  "Prefix" },
    { NODE_INFIX,                   "Infix" },
    { NODE_POSTFIX,                 "Postfix" },
    { NODE_STRING,                  "String" },
    { NODE_INTEGER,                 "Integer" },
};


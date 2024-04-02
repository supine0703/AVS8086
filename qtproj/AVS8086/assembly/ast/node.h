#ifndef NODE_H
#define NODE_H

#include <QMap>

class Node
{
public:
    enum Type {
        NODE_ILLEGAL = 0,
        NODE_PROGRAM,
        NODE_EXPRESSION_STATEMENT,
        NODE_PREFIX,
        NODE_INFIX,
        NODE_POSTFIX,
        NODE_STRING,
        NODE_INTEGER,
    };

public:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Type type);
    virtual ~Node();

    Type type() const;

    QString typeName() const;
    static QString nodeTypeName(Type type);

    virtual QStringList traversal(int depth) const = 0;


private:
    Type m_type;
    static const QMap<Type, QString> sm_names;
};

/* ========================================================================== */

class Expression : public Node
{
public:
    Expression(Type type) : Node(type) { }
    virtual ~Expression() { }
};

/* ========================================================================== */

class Statement : public Node
{
public:
    Statement(Type type) : Node(type) { }
    virtual ~Statement() { }
};

#endif // NODE_H

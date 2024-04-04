#ifndef NODE_H
#define NODE_H

#include <QVariant>
// #include <QMap>
// #include <QStringList>

namespace avs8086::ast {

class Node
{
public:
    enum Type {
        NODE_ILLEGAL = 0,

        NODE_INTEGER,   // integer
        NODE_STRING,    // string
        NODE_NAGATIVE,  // -x
        NODE_BIT_NOT,   // ~x
        NODE_REGISTER,  // reg
        NODE_ADDRESS,   // []
        NODE_INFIX,     // * / % + - & ^ | << >>
        NODE_COMMA,     // ,
        NODE_LABEL,     // label
        NODE_COLON,     // :
        // NODE_ASSIGN,    // =

        NODE_PROGRAM,
        NODE_EXPRESSION_STATEMENT,


        NODE_MOV,
    };

public:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Type type);
    virtual ~Node();

    Type type() const;
    bool isError() const;

    QString typeName() const;
    static QString nodeTypeName(Type type);

    virtual QStringList traversal(int depth) const = 0;

protected:
    void goError();

private:
    Type m_type;
    bool m_isError;
    static const QMap<Type, QString> sm_typeNames;
};

/* ========================================================================== */

class Expression : public Node
{
public:
    Expression(Type type) : Node(type) { }
    virtual ~Expression() { }


public:
    QVariant m_value;
};

/* ========================================================================== */

class Statement : public Node
{
public:
    Statement(Type type) : Node(type) { }
    virtual ~Statement() { }
};

} // namespace avs8086::ast

#endif // NODE_H

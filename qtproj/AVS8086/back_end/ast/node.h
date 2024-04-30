#ifndef NODE_H
#define NODE_H

#include <QSharedPointer>
#include <QJsonObject>
#include <QSet>

namespace avs8086::ast {

class Node
{
public:
    enum Type {
        ILLEGAL = 0,

        FLOAT,      // float
        INTEGER,    // integer
        STRING,     // string
        PREFIX,     // ~x +x -x
        INFIX,      // * / % + - & ^ | << >> = > >= < <= == !=
        COMMA,      // ,
        COLON,      // :

        REGISTER,   // reg
        ADDRESS,    // []

        PROGRAM,
        EXPRESSION_STATEMENT,
        MULTIPLE_STATEMENT,

        VARIABLE,   // variabel
        SEGMENT,    // segment
        LABEL,      // label

        WELL,       // #...#
        SINGLE,
        MOV,
    };

public:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Type type);
    virtual ~Node();

#if 0
    bool isError() const;
    void goError();
#endif

    bool is(Type type) const;
    Type type() const;

    QString typeName() const;
    static QString typeName(Type type);

    virtual QJsonObject json() const = 0;


private:
#if 0
    bool m_isError = false;
#endif
    Type m_type;
    static const QHash<Type, QString> sm_typeNames;
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

// 定义智能指针原型
using StmtPointer =  QSharedPointer<ast::Statement>;
using ExprPointer = QSharedPointer<ast::Expression>;

} // namespace avs8086::ast

#endif // NODE_H

#ifndef NODE_H
#define NODE_H

#include "ast/value.h"
#include "token/token.h"
#include <QSharedPointer>
#include <QJsonObject>
#include <QMap>
// #include <QStringList>

namespace avs8086::ast {

class Node
{
public:
    enum Type {
        NODE_ILLEGAL = 0,

        NODE_FLOAT,     // float
        NODE_INTEGER,   // integer
        NODE_STRING,    // string
        NODE_PREFIX,    // ~x +x -x
        NODE_INFIX,     // * / % + - & ^ | << >> = > >= < <= == !=
        NODE_COMMA,     // ,
        NODE_COLON,     // :
        NODE_LABEL,     // label
        NODE_REGISTER,  // reg
        NODE_ADDRESS,   // []

        NODE_PROGRAM,
        NODE_EXPRESSION_STATEMENT,
        NODE_MULTIPLE_STATEMENT,

        NODE_SINGLE,
        NODE_WELL,      // #
        NODE_MOV,
    };

public:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Type type);
    virtual ~Node();

    bool is(Type type) const;
    Type type() const;

    QString typeName() const;
    static QString nodeTypeName(Type type);

    virtual QStringList traversal(int depth) const { return {}; };
    virtual QJsonObject json() const = 0;

    bool isError() const { return m_isError; }
    void goError() { m_isError = true; }


public:
    token::Token m_token;


private:
    bool m_isError = false;
    Type m_type;
    static const QMap<Type, QString> sm_typeNames;
};

/* ========================================================================== */

class Expression : public Node
{
public:
    enum ValueType {
        ERROR = 0,
        REGISTER,
        FLOAT,
        STRING,
        INTEGER,
    };

public:
    Expression(Type type) : Node(type) { }
    virtual ~Expression() { }
    bool valueIs(ValueType type) const { return m_valueType == type;}
    ValueType valueType() const { return m_valueType; }
    Value value() const { return m_value; }

protected:
    ValueType m_valueType;
    Value m_value;
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

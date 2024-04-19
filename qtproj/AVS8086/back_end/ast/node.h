#ifndef NODE_H
#define NODE_H

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
        ILLEGAL = 0,

        FLOAT,     // float
        INTEGER,   // integer
        STRING,    // string
        PREFIX,    // ~x +x -x
        INFIX,     // * / % + - & ^ | << >> = > >= < <= == !=
        COMMA,     // ,
        COLON,     // :
        LABEL,     // label
        REGISTER,  // reg
        ADDRESS,   // []

        PROGRAM,
        EXPRESSION_STATEMENT,
        MULTIPLE_STATEMENT,

        SINGLE,
        WELL,      // #
        MOV,
    };

public:
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(Type type);
    virtual ~Node();

    bool is(Type type) const;
    Type type() const;

    QString typeName() const;
    static QString typeName(Type type);

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


protected:

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

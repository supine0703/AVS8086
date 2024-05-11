#ifndef NODE_H
#define NODE_H

#include "token/token.h"
#include <QJsonObject>
#include <QSharedPointer>

namespace avs8086::ast {

template <class X, class T>
inline QSharedPointer<X> assert_dynamic_cast(const QSharedPointer<T>& src)
{
    auto ptr = src.template dynamicCast<X>();
    return (Q_ASSERT(!ptr.isNull()), ptr);
}

template <class X, class T>
inline bool can_dynamic_cast(const QSharedPointer<T>& src)
{
    auto ptr = src.template dynamicCast<X>();
    return !ptr.isNull();
}

inline QByteArray show_Integer_hex(const QByteArray& bytes)
{
    auto tmp = bytes;
    std::reverse(tmp.begin(), tmp.end()); // 小端存储, 要颠倒
    return tmp.toHex();
}

/* ========================================================================== */

class Node
{
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

public:
    enum Type {
        NODE_EOL = -2,
        ILLEGAL = 0,

        DUP,
        VALUE,      // integer string
        FLOAT,      // float
        OPERATOR,   // * / % + - & ^ | << >> = > >= < <= == !=; ~x +x -x
        IDENTIFIER,

        WELL,       // #...#
        DEFINE,
        JMP,
        JX,

        COMMA,      // ,
        COMMA_ARRAY,
        COLON,      // :

        REGISTER,   // reg
        ADDRESS,    // []

        PROGRAM,
        EXPR_STMT,

        ASSIGN,
        MAKE_X,
        LOAD_X,

        SINGLE,
        MOV,
    };

protected:
    Node(Type type) : m_type(type) { }

public:
    virtual ~Node() = default;

    bool is(Type type) const { return m_type == type; }

    Type type() const { return m_type; }

    QString typeName() const { return typeName(m_type); }

    static QString typeName(Type type);

    virtual QJsonObject json() const = 0;


private:
    Type m_type;
    static const QHash<Type, QString> sm_typeNames;
};

// 定义智能指针原型
using StmtPointer = QSharedPointer<class Statement>;
using ExprPointer = QSharedPointer<class Expression>;

/* ========================================================================== */

class Statement : public Node
{
protected:
    Statement(Type type) : Node(type) { }

public:
    virtual ~Statement() = default;

    virtual QJsonObject json() const override;

    virtual void addIn(const StmtPointer& s, QList<StmtPointer>& stmts)
    { stmts.append(s); }
};

/* ========================================================================== */

class Expression : public Node
{
protected:
    // Expression(Type type) : Node(type) { }

    Expression(Type type, const token::Token& token)
        : Node(type), m_token(token)
    { }

public:
    virtual ~Expression() = default;

    virtual QJsonObject json() const override;

    token::Token token() const { return m_token; }

    virtual QByteArray bytes() const { return QByteArray(); }

    virtual int dataSize() const { return 0; } // 不可以用来判断是否可求值

    virtual int unitDataSize() const { return 0; } // 可以用来判断是否可求值

    virtual Position position() const { return m_token.position(); }

    virtual void addIn(const ExprPointer& e, QList<ExprPointer>& exprs)
    { exprs.append(e); }

protected:
    token::Token m_token;
};

} // namespace avs8086::ast

#endif // NODE_H

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
    enum Type  {
        NODE_EOL = -2,
        ILLEGAL = 0,

        DUP,
        VALUE,      // integer string
        FLOAT,      // float
        OPERATOR,   // * / % + - & ^ | << >> = > >= < <= == !=; ~x +x -x
        IDENTIFIER,

        WELL,       // #...#
        DEFINE,

        SINGLE,

        COMMA,      // ,
        COMMA_ARRAY,
        COLON,      // :

        REGISTER,   // reg
        REG_UNION,  // reg union
        ADDRESS,    // []

        PROGRAM,
        EXPR_STMT,

        ASSIGN,
        MAKE_X,
        LOAD_X,

        MOV,
        PUSH,
        POP,
        XCHG,
        LEA,
        LDS,
        LES,

        NOT,
        AND,
        OR,
        XOR,
        TEST,
        SAL,
        SAR,
        SHL,
        SHR,
        ROL,
        ROR,
        RCL,
        RCR,

        XLAT,
        LAHF,
        SAHF,
        PUSHF,
        POPF,
        CBW,
        CWD,
        AAA,
        DAA,
        AAS,
        DAS,
        INTO,
        IRET,
        REP,
        REPE,
        REPZ,
        REPNE,
        REPNZ,
        MOVSB,
        MOVSW,
        CMPSB,
        CMPSW,
        SCASB,
        SCASW,
        LODSB,
        LODSW,
        STOSB,
        STOSW,
        CLC,
        STC,
        CMC,
        CLD,
        STD,
        CLI,
        STI,
        WAIT,
        LOCK,
        HLT,
        NOP,
        AAM,
        AAD,

        JMP,
        JO,
        JNO,
        JB,
        JC,
        JNAE,
        JAE,
        JNB,
        JNC,
        JE,
        JZ,
        JNE,
        JNZ,
        JBE,
        JNA,
        JA,
        JNBE,
        JS,
        JNS,
        JP,
        JPE,
        JNP,
        JPO,
        JL,
        JNGE,
        JGE,
        JNL,
        JLE,
        JNG,
        JG,
        JNLE,
        JCXZ,
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

    QByteArray m_codes;

public:
    virtual ~Statement() = default;

    virtual QJsonObject json() const override;

    virtual void addIn(const StmtPointer& s, QList<StmtPointer>& stmts)
    { stmts.append(s); }

    virtual QByteArray codes() const { return m_codes; }

    qsizetype length() const { return m_codes.length(); }
};

/* ========================================================================== */

class Expression : public Node
{
protected:
    Expression(Type type, const token::Token& token)
        : Node(type), m_token(token)
    { }

    token::Token m_token;

public:
    virtual ~Expression() = default;

    virtual QJsonObject json() const override;

    token::Token token() const { return m_token; }

    virtual QByteArray bytes() const { return QByteArray(); }

    virtual int dataSize() const { return 0; } // 不可以用来判断是否可求值

    virtual int unitDataSize() const { return 0; } // 可以用来判断是否可求值

    virtual Position pos() const { return m_token.pos(); }

    virtual void addIn(const ExprPointer& e, QList<ExprPointer>& exprs)
    { exprs.append(e); }
};

} // namespace avs8086::ast

#endif // NODE_H

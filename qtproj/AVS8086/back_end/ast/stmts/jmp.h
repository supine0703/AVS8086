#ifndef JMP_H
#define JMP_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Jmp : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Jmp(const token::Token& id)
        : Statement(JMP), m_id(id)
    { }

    ~Jmp() = default;

    virtual QJsonObject json() const override;

private:
    void setOffset(int offset)
    { // 传入假设自身大小为零的偏移
        if (m_offset == offset)
            return ;
        m_codes.clear();
#if 0
        for (auto it = sm_sizeMax.begin(); it != sm_sizeMax.end(); it++)
        {
            if ((it.key() - 2 + it.value()) < offset && offset <= it.value())
            {
                m_size = it.key();
                break;
            }
        }
#else
        if (-0x7f <= offset && offset < 0x7f)
        {
            m_size = 2;
            if (offset < 0)
            {
                offset--;
            }
            m_codes.append(0xeb);
            m_codes.append(static_cast<char>(offset & 0xff));
        }
        else if (-0x7ffe <= offset && offset < 0x7fff)
        {
            m_size = 3;
            if (offset < 0)
            {
                offset -= 2;
            }
            m_codes.append(0xe9);
            m_codes.append(static_cast<char>(offset & 0xff));
            m_codes.append(static_cast<char>((offset >> 8) & 0xff));
        }
        else
        {
            m_size = 5;
            m_codes.append(0xea);
        }
        m_offset = offset;
#endif
    }

    token::Token m_id;
    int m_size;
    int m_offset;

#if 0
    static const QMap<int ,int> sm_sizeMax;
#endif
};

#if 0
const QMap<int ,int> Jmp::sm_sizeMax = {
    { 2, 0x7f }, { 3, 0x7fff }, { 5, 0x7ffff }
};
#endif

inline QJsonObject Jmp::json() const
{
    QJsonObject js = Statement::json();
    js["call"] = *m_id;
    js["size"] = m_size;
    js["offset"] = m_offset;
    return js;
}

/* ========================================================================== */

class Jx : public Statement
{
    using Pointer = QSharedPointer<Jx>;
    friend class avs8086::parser::Parser;
public:
    ~Jx() = default;

    virtual QJsonObject json() const override;

    static Pointer NEW(token::Token::Type type)
    {
        auto it = sm_constructors.find(type);
        if (it != sm_constructors.end())
        {
            return (*sm_constructors.value(type))();
        }
        return Pointer(nullptr);
    }

private:
    bool setOffset(int offset)
    {
        m_offset = offset;
        m_codes.clear();
        m_codes.append(m_code);
        m_codes.append(static_cast<char>(offset & 0xff));
        return (-0x80 <= offset) && (offset <= 0x7f);
    }

    token::Token m_id;
    Position m_pos;
    int m_offset;
    int8_t m_code;

    typedef Pointer (*constructor)(void);
    static const QHash<token::Token::Type, constructor> sm_constructors;

    Jx(Type type, int8_t code) : Statement(type), m_code(code) { }
};

inline QJsonObject Jx::json() const
{
    QJsonObject js = Statement::json();
    js["call"] = *m_id;
    js["offset"] = m_offset;
    return js;
}

/* ========================================================================== */

#if 0
class Jx : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Jx(const token::Token& token, const token::Token& id)
        : Statement(JX)
        , m_id(id)
        , m_literal((*token).toUpper())
        , m_pos(token.pos() + id.pos())
    {
        Q_ASSERT(sm_toCodes.contains(token.type()));
        m_codes.append(sm_toCodes.value(token.type()));
    }

    ~Jx() = default;

    virtual QJsonObject json() const override;

private:
    bool setOffset(int offset)
    {
        m_offset = offset;
        m_codes.append(static_cast<char>(offset & 0xff));
        return -0x80 <= offset && offset <= 0x7f;
    }

    token::Token m_id;
    QString m_literal;
    Position m_pos;
    int m_offset;

    static const QHash<token::Token::Type, char> sm_toCodes;

    typedef QSharedPointer<Jx> (*constructor)(const token::Token&);
    static const QHash<token::Token::Type, constructor> sm_constructors;
};

inline QJsonObject Jx::json() const
{
    QJsonObject js = Statement::json();
    js["token"] = m_literal;
    js["call"] = *m_id;
    js["offset"] = m_offset;
    return js;
}
#endif

} // namespace avs8086::ast

#endif // JMP_H

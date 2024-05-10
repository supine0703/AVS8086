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
        if (-0x7f < offset && offset <= 0x7f)
        {
            m_size = 2;
        }
        else if (-0x7ffe < offset && offset <= 0x7fff)
        {
            m_size = 3;
        }
        else
        {
            m_size = 5;
        }
        m_offset = (offset < 0) ? (offset - m_size) : (offset);
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

class Jx : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Jx(const token::Token& token, const token::Token& id)
        : Statement(JX)
        , m_id(id)
        , m_literal(*token)
        , m_pos(id.row(), token.column(), id.endColumn() - token.column())
    { }

    ~Jx() = default;

    virtual QJsonObject json() const override;

private:
    bool setOffset(int offset)
    {
        m_offset = offset;
        return -0x80 <= offset && offset <= 0x7f;
    }

    token::Token m_id;
    QString m_literal;
    Position m_pos;
    int m_offset;
};

inline QJsonObject Jx::json() const
{
    QJsonObject js = Statement::json();
    js["token"] = m_literal;
    js["call"] = *m_id;
    js["offset"] = m_offset;
    return js;
}

} // namespace avs8086::ast

#endif // JMP_H

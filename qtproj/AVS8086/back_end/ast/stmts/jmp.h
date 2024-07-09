#ifndef JMP_H
#define JMP_H

#include "ast/stmts/instruction.h"

namespace avs8086::ast {

class JmpBase : public Instruction
{
public:
    using Pointer = QSharedPointer<JmpBase>;

    ~JmpBase() = default;

    virtual QJsonObject json() const override;

    virtual bool setOffset(int offset) = 0;

    virtual bool setSegOffset(int seg, int offset) { return false; }

    QString label() const { return m_label; }

    Position pos() const { return m_pos; }

    int offset() const { return m_offset; }

    int labelSize() const { return m_size; }

private:
    QString m_label;
    Position m_pos;

protected:
    int m_offset = 0;
    int m_size;

    JmpBase(const token::Token& j, const token::Token& id, int size)
        : Instruction(j.type())
        , m_label(*id)
        , m_pos(j.pos() + id.pos())
        , m_size(size)
    { }
};

inline QJsonObject JmpBase::json() const
{
    QJsonObject js = Instruction::json();
    js["jmp"] = m_label;
    js["size"] = m_size;
    js["offset"] = m_offset;
    return js;
}

/* ========================================================================== */

class Jmp : public JmpBase
{ // 直接跳转
public:
    using Pointer = QSharedPointer<Jmp>;

    enum Code : uint8_t {
        SHORT     = 0xeb,
        NEAR_PTR  = 0xe9,
        FAR_PTR   = 0xea,
    };

    Jmp(const token::Token& jmp, const token::Token& id, int size = 0)
        : JmpBase(jmp, id, size), fixed(size != 0)
    { }

    ~Jmp() = default;

    virtual bool setSegOffset(int seg, int offset) override
    {
        if (m_size == 5
            && m_code.size() == 1
            && m_code.at(0) == static_cast<char>(FAR_PTR))
        {
            m_code.append(static_cast<char>(offset & 0xff));
            m_code.append(static_cast<char>((offset >> 8) & 0xff));
            m_code.append(static_cast<char>(seg & 0xff));
            m_code.append(static_cast<char>((seg >> 8) & 0xff));
            return true;
        }
        return false;
    }

    virtual bool setOffset(int offset) override
    { // 传入假设自身大小为零的偏移
        if (m_offset == offset && !m_code.isEmpty())
        {
            return lastRet;
        }

        m_code.clear();

        bool neg = offset < 0;
        offset += neg;

        int size;
        if (-0x7f < offset && offset <= 0x7f)
        {
            size = 2;
        }
        else if (-0x7ffe < offset && offset <= 0x7fff)
        {
            size = 3;
        }
        else
        {
            size = 5;
        }

        if (neg)
        { // 向前寻找, 偏移加上自身大小(负数)
            offset += -size;
        }
        m_offset = offset;

        if (!fixed)
        {
            m_size = size;
        }

        if (m_size == 2)
        {
            m_code.append(SHORT);
            m_code.append(static_cast<char>(offset & 0xff));
        }
        else if (m_size == 3)
        {
            m_code.append(NEAR_PTR);
            m_code.append(static_cast<char>(offset & 0xff));
            m_code.append(static_cast<char>((offset >> 8) & 0xff));
        }
        else if (m_size == 5)
        {
            m_code.append(FAR_PTR);
            return false; // then setSegOffset
        }
        else
        {
            Q_ASSERT_X(false, "Jmp::setOffset", "size error");
        }
        return (lastRet = (m_size >= size));
    }

private:
    bool fixed;
    bool lastRet;
};

/* ========================================================================== */

#if 1
class Jx : public JmpBase
{ // loop指令性质完全相同, 也归于同一类
public:
    using Pointer = QSharedPointer<Jx>;

    Jx(const token::Token& jx, const token::Token& id)
        : JmpBase(jx, id, 2)
        , m_jxCode((Q_ASSERT(sm_codes.contains(jx.type()))
                    , sm_codes.value(jx.type())))
    { }

    ~Jx() = default;

    virtual bool setOffset(int offset) override
    {
        if (!(m_offset == offset && !m_code.isEmpty()))
        {
            m_offset = offset;
            m_code.clear();
            m_code.append(m_jxCode);
            m_code.append(static_cast<char>(offset & 0xff));
        }
        return ((-0x80 <= offset) && (offset <= 0x7f));
    }

private:
    char m_jxCode;
    static const QHash<token::Token::Type, char> sm_codes;
};

#endif

/* ========================================================================== */

#if 0
class Jx : public Instruction
{
    using Pointer = QSharedPointer<Jx>;
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
        m_code.clear();
        m_code.append(m_idCode);
        m_code.append(static_cast<char>(offset & 0xff));
        return (-0x80 <= offset) && (offset <= 0x7f);
    }

    token::Token m_id;
    Position m_pos;
    int m_offset;
    int8_t m_idCode;

    typedef Pointer (*constructor)(void);
    static const QHash<token::Token::Type, constructor> sm_constructors;

    Jx(Type type, int8_t code) : Statement(type), m_idCode(code) { }
};

inline QJsonObject Jx::json() const
{
    QJsonObject js = Statement::json();
    js["call"] = *m_id;
    js["offset"] = m_offset;
    return js;
}
#endif

} // namespace avs8086::ast

#endif // JMP_H

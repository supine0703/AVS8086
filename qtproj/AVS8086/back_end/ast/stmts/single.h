#ifndef SINGLE_H
#define SINGLE_H

#include "ast/stmts/instruction.h"

namespace avs8086::ast {

class Single : public Instruction
{
public:
    using Pointer = QSharedPointer<Single>;

    Single(token::Token::Type type)
        : Instruction(type)
    {
        Q_ASSERT(sm_codes.contains(type));
        m_code.append(sm_codes.value(type));
    }

    ~Single() = default;

private:
    using initByteList = std::initializer_list<uint8_t>;
    static const QHash<token::Token::Type, initByteList> sm_codes;
};

#if 0
class Single : public Statement
{
    using Pointer = QSharedPointer<Single>;
public:
    static Pointer NEW(token::Token::Type type)
    {
        auto it = sm_constructors.find(type);
        if (it != sm_constructors.end())
        {
            return (*sm_constructors.value(type))();
        }
        return Pointer(nullptr);
    }

    ~Single() = default;

private:
    typedef Pointer (*constructor)(void);
    static const QHash<token::Token::Type, constructor> sm_constructors;

    Single(Type type, int8_t code) : Statement(type) { m_codes.append(code); }

    Single(Type type, std::initializer_list<uint8_t> codes)
        : Statement(type)
    { m_codes.append(codes); }
};
#endif
} // namespace avs8086::ast

#endif // SINGLE_H

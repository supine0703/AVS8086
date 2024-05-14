#ifndef SINGLE_H
#define SINGLE_H

#include "ast/node.h"

namespace avs8086::ast {

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

} // namespace avs8086::ast

#endif // SINGLE_H

#ifndef LXX_H
#define LXX_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Multiple : public Statement
{
    using Pointer = QSharedPointer<Multiple>;
    friend class avs8086::parser::Parser;
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

    ~Multiple() = default;

private:
    ExprPointer m_expr;

    typedef Pointer (*constructor)(void);
    static const QHash<token::Token::Type, constructor> sm_constructors;

    Multiple(Type type, std::initializer_list<uint8_t> codes)
        : Statement(type)
    { m_codes.append(codes); }
};

} // namespace avs8086::ast

#endif // LXX_H

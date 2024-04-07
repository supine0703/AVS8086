#ifndef SINGLE_HPP
#define SINGLE_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Single : public Statement
{
public:
    Single(const token::Token& token)
        : Statement(NODE_SINGLE), m_token(token)
    { }
    ~Single() { }

    QStringList traversal(int depth) const override
    {
        return {
            QString("%1| %2: %3").arg(
                QString(depth * 4, '-'), typeName(), m_token.literal().toUpper()
            )
        };
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        return js;
    }


public:
    token::Token m_token;
};

} // namespace avs8086::ast

#endif // SINGLE_HPP

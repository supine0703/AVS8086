#ifndef SINGLE_H
#define SINGLE_H

#include "ast/node.h"

namespace avs8086::ast {

class Single : public Statement
{
public:
    Single(const token::Token& token) : Statement(SINGLE), m_token(token) { }

    ~Single() = default;

    virtual QJsonObject json() const override;


private:
    token::Token m_token;
};

inline QJsonObject Single::json() const
{
    QJsonObject js = Statement::json();
    js["token"] = *m_token;
    return js;
}

} // namespace avs8086::ast


#endif // SINGLE_H

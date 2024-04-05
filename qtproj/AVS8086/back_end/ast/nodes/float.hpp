#ifndef FLOAT_HPP
#define FLOAT_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Float : public Expression
{
public:
    Float(const token::Token& token)
        : Expression(NODE_FLOAT)
        , m_value(token.literal().toDouble())
    { }
    ~Float() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), QString("%1").arg(m_value)
        ));
        return info;
    }

private:
    double m_value;
};

} // namespace avs8086::ast

#endif // FLOAT_HPP

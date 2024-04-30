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
    {
        m_valueType = FLOAT;
        m_value = QString::number(token.literal().toDouble());
        goError();
    }
    ~Float() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), "no value"
        ));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        return js;
    }
};

} // namespace avs8086::ast

#endif // FLOAT_HPP

#ifndef WELL_H
#define WELL_H

#include "ast/node.h"

namespace avs8086::ast {

class Well : public Statement
{
public:
    Well() : Statement(WELL) { }

    Well(const ExprPointer& expr, const QString& key, const QString& value)
        : Statement(WELL), m_expr(expr), m_key(key), m_value(value)
    { }

    ~Well() = default;

    virtual QJsonObject json() const override;

    QString key() const { return m_key; }

    QString value() const { return m_value; }

private:
    QString m_key;
    QString m_value;
    ExprPointer m_expr;
};

} // namespace avs8086::ast

#endif // WELL_H

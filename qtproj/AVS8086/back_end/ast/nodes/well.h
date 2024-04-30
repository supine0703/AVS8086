#ifndef WELL_H
#define WELL_H

#include "ast/node.h"

namespace avs8086::ast {

class Well : public Statement
{
public:
    Well() : Statement(WELL) { }
    ~Well() { }

    QJsonObject json() const override;

public:
    QString m_instruction;
    QString m_key;
    QString m_value;
};

} // namespace avs8086::ast

#endif // WELL_H

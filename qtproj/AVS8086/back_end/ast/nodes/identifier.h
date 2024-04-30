#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "ast/node.h"

namespace avs8086::ast {

class Variable : public Statement
{
public:
    Variable() : Statement(VARIABLE) { }
    ~Variable() { }

    QJsonObject json() const override;

public:

    ExprPointer m_expr;
};

/* ========================================================================== */

class Segment : public Statement
{
public:
    Segment() : Statement(SEGMENT) { }
    ~Segment() { }

    QJsonObject json() const override;

public:
};

/* ========================================================================== */

class Label : public Statement
{
public:
    Label() : Statement(LABEL) { }
    ~Label() { }

    QJsonObject json() const override;

public:
};

} // namespace avs8086::ast


#endif // IDENTIFIER_H

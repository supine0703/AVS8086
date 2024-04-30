#ifndef PROGRAM_H
#define PROGRAM_H

#include "ast/node.h"

namespace avs8086::ast {

class Program : public Statement
{
public:
    Program(const QString& file)
        : Statement(PROGRAM), m_file(file)
    { }
    ~Program() { }

    QString file() const { return m_file; }

    QJsonObject json() const override;

public:
    QList<QSharedPointer<Statement>> m_statements;

private:
    QString m_file;
};

} // namespace avs8086::ast

#endif // PROGRAM_H

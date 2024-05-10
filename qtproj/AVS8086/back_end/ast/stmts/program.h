#ifndef PROGRAM_H
#define PROGRAM_H

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class Program : public Statement
{
    friend class avs8086::parser::Parser;
public:
    Program(const QString& file = "") : Statement(PROGRAM), m_file(file) { }

    ~Program() = default;

    virtual QJsonObject json() const override;

    virtual void addIn(const StmtPointer& s, QList<StmtPointer>& stmts) override
    { Q_UNUSED(s); stmts.append(m_stmts); }

    bool haveError() const { return m_err; }

    QString file() const { return m_file; }

    QList<StmtPointer> statements() const { return m_stmts; }

private:
    bool m_err;
    QString m_file;
    QList<StmtPointer> m_stmts;
};

} // namespace avs8086::ast

#endif // PROGRAM_H

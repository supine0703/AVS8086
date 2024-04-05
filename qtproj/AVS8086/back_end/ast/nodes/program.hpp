#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "ast/node.h"

namespace avs8086::ast {

class Program : public Statement
{
public:
    Program(const QString& file)
        : Statement(NODE_PROGRAM), m_file(file)
    { }
    ~Program() { }

    QStringList traversal() const { return traversal(0); }
    
    QStringList traversal(int depth) const override
    {
        QStringList tree;
        tree.append(
            QString("%1| %2(%4): %3")
                .arg(QString(depth * 4, '-'), typeName(), m_file)
                .arg(m_statements.length())
        );
        for (const auto& s : m_statements)
            tree.append(s->traversal(depth + 1));
        return tree;
    }

    QString file() const { return m_file; }

    QList<QSharedPointer<Statement>> statements() const
    { return m_statements; }

    void append(const QSharedPointer<Statement>& statement)
    { m_statements.append(statement); }

    void append(const QList<QSharedPointer<Statement>>& statements)
    { m_statements.append(statements); }


private:
    QString m_file;
    QList<QSharedPointer<Statement>> m_statements;
};

} // namespace avs8086::ast

#endif // PROGRAM_H

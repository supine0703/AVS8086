#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "ast/node.h"
#include "token/token.h"
#include <QSharedPointer>


class Program : public Statement
{
public:
    Program(const Token& token = Token())
        : Statement(NODE_PROGRAM), m_file(token.literal())
    { }
    ~Program() { }

    QStringList ergodic() const { return traversal(0); }
    
    QStringList traversal(int depth) const override
    {
        QStringList tree;
        tree.append(QString("%1| %2(%4): %3").arg(
            QString(depth * 4, '-'), typeName(), m_file
        ).arg(m_statements.length()));
        for (const auto& s : m_statements)
            tree.append(s->traversal(depth + 1));
        return tree;
    }

    void append(const QSharedPointer<Statement>& statement)
    { m_statements.append(statement); }


private:
    QString m_file;
    QList<QSharedPointer<Statement>> m_statements;
};


#endif // PROGRAM_H

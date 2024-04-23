#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include "ast/node.h"
#include <QSharedPointer>

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

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["file"] = m_file;
        QJsonObject stmts;
        for (int i = 0; i < m_statements.length(); i++)
        {
            const auto& s = m_statements.at(i);
            stmts[QString("stmt %1").arg(i + 1)] = s->json();
        }
        js["statements"] = stmts;
        return js;
    }

    QString file() const { return m_file; }

public:
    QList<QSharedPointer<Statement>> m_statements;


private:
    QString m_file;
};

} // namespace avs8086::ast

#endif // PROGRAM_H

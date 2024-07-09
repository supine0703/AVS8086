#ifndef MULTIPLE_STATEMENT_HPP
#define MULTIPLE_STATEMENT_HPP

#include "ast/node.h"

namespace avs8086::ast {

class MultipleStatement : public Statement
{
public:
    MultipleStatement() : Statement(NODE_MULTIPLE_STATEMENT) { }
    ~MultipleStatement() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2").arg(
            QString(depth * 4, '-'), typeName()
        ));
        depth++;
        for (const auto& s : m_statements)
            info.append(s->traversal(depth));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        QJsonObject stmts;
        for (const auto& s : m_statements)
        {
            stmts[s->typeName()] = s->json();
        }
        js["statements"] = stmts;
        return js;
    }

public:
    void append(const QSharedPointer<Statement>& statement)
    {
        if (statement->is(NODE_MULTIPLE_STATEMENT))
        {
            m_statements.append(
                qSharedPointerDynamicCast<MultipleStatement>
                (statement)->m_statements
            );
        }
        else
            m_statements.append(statement);
    }

public:
    QList<QSharedPointer<Statement>> m_statements;
};

} // namespace avs8086::ast

#endif // MULTIPLE_STATEMENT_HPP

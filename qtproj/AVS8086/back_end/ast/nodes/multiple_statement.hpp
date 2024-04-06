#ifndef MULTIPLE_STATEMENT_HPP
#define MULTIPLE_STATEMENT_HPP

#include "ast/node.h"

namespace avs8086::parser {
class Parser;
} // namespace avs8086::parser

namespace avs8086::ast {

class MultipleStatement : public Statement
{
    friend class avs8086::parser::Parser;
public:
    MultipleStatement() : Statement(NODE_MULTIPLE_STATEMENT) { }
    ~MultipleStatement() { }

    QStringList traversal(int depth) const override
    {
        // if (isError())
        //     return {
        //         QString("%1| %2: member is false!")
        //             .arg(QString(depth * 4, '-'), typeName())
        //     };
        QStringList info;
        info.append(QString("%1| %2").arg(
            QString(depth * 4, '-'), typeName()
        ));
        depth++;
        for (const auto& s : m_statements)
            info.append(s->traversal(depth));
        return info;
    }

    QList<QSharedPointer<Statement>> statements() const
    { return m_statements; }

private:
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

private:
    QList<QSharedPointer<Statement>> m_statements;
};

} // namespace avs8086::ast

#endif // MULTIPLE_STATEMENT_HPP

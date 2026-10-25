#ifndef PROGRAM_H
#define PROGRAM_H

#include "ast/node.h"

namespace lzl::a1::parser {
class Parser;
} // namespace lzl::a1::parser

namespace lzl::a1::ast {

class Program : public Statement
{
    friend class lzl::a1::parser::Parser;

public:
    Program(const QString& file = "") : Statement(PROGRAM), m_file(file) {}

    ~Program() = default;

    virtual QJsonObject json() const override;

    bool haveError() const { return m_err; }

    QString file() const { return m_file; }

    QList<StmtPointer> stmts() const { return m_stmts; }

private:
    bool m_err;
    QString m_file;
    QList<StmtPointer> m_stmts;
};

inline QJsonObject Program::json() const
{
    QJsonObject js = Statement::json();
    js["file"] = m_file;
#if 0
    QJsonObject symbols;
    for (const auto& id : m_identifiers.list())
    {
        QJsonObject id_js;
        id_js["offset"] = id.offset();
        id_js["size"] = id.size();
        id_js["type"] = id.typeName();
        id_js["segment"] = id.segment();
        symbols[id.id()] = id_js;
    }
js["symbols"] = symbols;
#endif
    // QJsonArray stmts;
    // for (int i = 0; i < m_stmts.size(); i++)
    // {
    //     QJsonObject s;
    //     s[QString("stmt %1").arg(i + 1)] = m_stmts.at(i)->json();
    //     stmts.append(s);
    // }
    QJsonObject stmts;
    auto char_len = QString::number(m_stmts.size() - 1).size();
    for (int i = 0; i < m_stmts.size(); i++)
    {
        stmts[QString("stmt %1").arg(i + 1, char_len)] = m_stmts.at(i)->json();
    }
    js["statements"] = stmts;
    return js;
}

} // namespace lzl::a1::ast

#endif // PROGRAM_H

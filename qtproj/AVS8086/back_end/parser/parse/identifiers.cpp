#include "parser/parser.h"
#include "ast/stmts/define.h"
#include "ast/stmts/jmp.h"

using namespace avs8086;
using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

const QHash<Token::Type, Identifier::Type> Parser::sm_idTypes = {
    { Token::COLON,     Identifier::LABEL },
    { Token::SEGMENT,   Identifier::SEGMENT },
    { Token::PROC_NEAR, Identifier::NEAR },
    { Token::PROC_FAR,  Identifier::FAR },
};

/* ========================================================================== */

void Parser::parse_idTable()
{
    if (m_offsets.size() == 0)
        return;
    if (parse_idTable(m_idTable, -1, m_offsets.size() - 1, 0, 0xfffff))
        ;
    for (const auto& [offset, s] : m_lastCalls)
    { // 最后在计算地址, 包括 jmp far, jmp ...(ptr), jx
        auto jb = assert_dynamic_cast<JmpBase>(s);
        auto id = m_idTable.find(jb->label());
        Q_ASSERT(id != m_idTable.end());
        if (jb->labelSize() == 5)
        { // jmp far
            if (!jb->setSegOffset(0, id->offset()))
            {
                Q_ASSERT_X("false", "Parser::parse_idTable", "jmp far error");
            }
        }
        else
        { // jx, jmp short, jmp near ptr
            if (!jb->setOffset(id->offset() - offset))
            {
                if (jb->labelSize() == 2)
                {
                    addJmpOverflowErrorInfo(jb->pos(), -128, 127);
                }
                else if (jb->labelSize() == 3)
                {
                    addJmpOverflowErrorInfo(jb->pos(), -32769, 32767);
                }
                else
                {
                    Q_ASSERT_X(false, "Parser::parse_idTable", "");
                }
            }
        }
    }
    m_currOffset = 0;
}

/* ========================================================================== */

bool Parser::parse_idTable(IdTable& t, int begin, int end, int offset, int max)
{
    Q_ASSERT(begin < end);

    IdTable table = t;
    int differ = 0;

    for (int i = begin + 1; i <= end; i++)
    {
        differ += m_offsets.at(i);
        auto it = m_ids.find(i);
        if (it != m_ids.end())
        { // 定义
            auto d = assert_dynamic_cast<Define>(it.value());
            if (d->m_op.is(Token::ALLOCATE))
            {
                table.insert(
                    {d->m_id, d->m_expr->dataSize(), offset + differ, m_currSeg}
                );
            }
            else
            {
                Q_ASSERT(sm_idTypes.contains(d->m_op.type()));
                auto type = sm_idTypes.value(d->m_op.type());
                table.insert({d->m_id, type, offset + differ, m_currSeg});
            }
        }
        else
        { // 调用
            Q_ASSERT(m_calls.contains(i));
            auto s = m_calls.value(i);
            auto jb = s.dynamicCast<JmpBase>();
            if (!jb.isNull())
            {
                if (jb->labelSize() != 0)
                { // jx, jmp ...(ptr)
                    // TODO: jmp far
                    Q_ASSERT(jb->labelSize() != 5);
                    differ += jb->labelSize();
                    m_lastCalls.append({offset + differ, s});
                    continue;
                }
                // jmp
                auto id = table.find(jb->label());
                if (id != table.end())
                { // 向前寻找
                    Q_ASSERT(
                        jb->setOffset(id->offset() - (offset + differ + 1))
                    );
                    jb->setOffset(id->offset() - (offset + differ + 1));
                    differ += jb->labelSize();
                }
                else
                { // 向后寻找
                    Q_ASSERT(m_idIts.contains(jb->label()));
                    auto id_i = m_idIts.value(jb->label());
                    int tmp = offset + differ;
                    if (parse_idTable(table, i, id_i, tmp + 2, 0x7f))
                    {
                        differ += 2;
                    }
                    else if (parse_idTable(table, i, id_i, tmp + 3, 0x7fff))
                    {
                        differ += 3;
                    }
                    else
                    {
                        differ += 5;
                        m_lastCalls.append({offset + differ, s});
                    }
                    id = table.find(jb->label());
                    Q_ASSERT(id != table.end());
                    Q_ASSERT(id->offset() - (offset + differ) >= 0);
                    Q_ASSERT(
                        jb->setOffset(id->offset() - (offset + differ))
                        || jb->labelSize() == 5
                    );
                    jb->setOffset(id->offset() - (offset + differ));
                }
            }
            else
            {
                // TODO: call, ret
                Q_ASSERT_X(false, "Parser::parse_idTable", "not jmp");
            }
        }
        if (differ > max)
        {
            qDebug() << differ;
            return false;
        }
    }

    m_currOffset = offset + differ;
    t.swap(table);
    return true;
}

/* ========================================================================== */

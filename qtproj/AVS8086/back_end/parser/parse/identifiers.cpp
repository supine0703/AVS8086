#include "parser/parser.h"
#include "ast/stmts/define.h"
#include "ast/stmts/jmp.h"

using namespace avs8086;
using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

const QHash<Token::Type, Identifier::Type> Parser::sm_idTypes = {
    { Token::COLON,     Identifier::LABEL },
    { Token::SEGMENT,   Identifier::SEGMENT },
    { Token::PROC_NEAR, Identifier::NEAR },
    { Token::PROC_FAR,  Identifier::FAR },
};

void Parser::parse_idTable()
{
    if (parse_idTable(m_idTable, -1, m_offsers.count() - 1, 0, 0xfffff))
        ;
    for (const auto& [offset, s] : m_lastCalls)
    {
        if (s->is(Node::JX))
        {
            auto jx = assert_dynamic_cast<Jx>(s);
            auto id = m_idTable.find(*(jx->m_id));
            Q_ASSERT(id != m_idTable.end());
            if (!jx->setOffset(id->offset() - offset))
            {
                addJmpOverflowErrorInfo(jx->m_pos, -128, 127);
            }
        }
        else if (s->is(Node::JMP))
        {
            auto jmp = assert_dynamic_cast<Jmp>(s);
            auto id = m_idTable.find(*(jmp->m_id));
            Q_ASSERT(id != m_idTable.end());
            jmp->setOffset(id->offset() - offset);
        }
    }
}

bool Parser::parse_idTable(IdTable& t, int begin, int end, int offset, int max)
{
    Q_ASSERT(begin < end);

    IdTable table = t;
    int differ = 0;

    for (int i = begin + 1; i <= end; i++)
    {
        differ += m_offsers.at(i);
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
            if (s->is(Node::JX))
            {
                differ += 2;
                m_lastCalls.append({offset + differ, s});
                continue;
            }
            else if (s->is(Node::JMP))
            {
                auto jmp = assert_dynamic_cast<Jmp>(s);
                auto id = table.find(*(jmp->m_id));
                if (id != table.end())
                { // 向前寻找
                    jmp->setOffset(id->offset() - (offset + differ));
                    differ += jmp->m_size;
                }
                else
                { // 向后寻找
                    Q_ASSERT(m_idIts.contains(*(jmp->m_id)));
                    auto id_i = m_idIts.value(*(jmp->m_id));
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
                        continue;
                    }
                    id = table.find(*(jmp->m_id));
                    Q_ASSERT(id != table.end());
                    jmp->setOffset(id->offset() - (offset + differ));
                }
            }
        }
        if (differ > max)
            return false;
    }

    m_currOffset = offset + differ;
    t.swap(table);
    return true;
}

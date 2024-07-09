#include "parser/parser.h"
#include "ast/stmts/define.h"
#include "ast/exprs/value.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_allocate()
{
    QSharedPointer<Define> d;
    if (currToken().is(Token::IDENTIFIER))
    { // 定义标识符 label dx ...
        d.reset(new Define(*currToken(), peekToken()));
        defineId(d);
        nextToken();
    }
    else
    { // 不定义标识符, 仅开空间 dx ...
        d.reset(new Define("", currToken()));
    }

    // Q_ASSERT(currToken().is(Token::ALLOCATE));

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    { // 不能没有右值, 遇到换行报错返回
        addExpectExprErrorInfo(
            parse_illegal(peekToken()), {Node::VALUE, Node::DUP, Node::COMMA}
        );
        return d;
    }

    static const QHash<QString, int> s_unitSize = {
        { "DB", 1 }, { "DW", 2 }, { "DD", 4 }, { "DQ", 8 }, { "DT", 10 },
    };
    Q_ASSERT(s_unitSize.contains((*(d->m_op)).toUpper()));
    m_currUnitSize = s_unitSize.value((*(d->m_op)).toUpper());
    auto max = ~(static_cast<size_t>(-1) << (m_currUnitSize << 3));

    d->m_expr = parse_expression();
    auto& e = d->m_expr;

    if (e->is(Node::COMMA))
    {
        auto ca = parse_commaArray(e);
        if (!ca.isNull())
        {
            e = ca;
        }
    }
    else if (!expectExprAbleToEvaluate(e))
    {
    }
    else if (e->is(Node::DUP))
    {
    }
    else if (e->is(Node::VALUE))
    { // 处理Value
        auto v = assert_dynamic_cast<Value>(e);
        // 检查数据长度
        if (v->unitDataSize() > m_currUnitSize)
        {
            addValueOverflowErrorInfo(v, max);
        }
        // 对齐数据
        v->alignData(m_currUnitSize);
#if 0
        auto post = v->dataSize() % m_currUnitSize;
        if (post != 0)
        {
            v->m_value.append(m_currUnitSize - post, 0);
        }
#endif
    }
    else
    {
        addExpectExprErrorInfo(e, {Node::COMMA, Node::DUP, Node::VALUE});
        return d;
    }

    m_currOffset += d->m_expr->dataSize();

    return d;
}

/* ========================================================================== */

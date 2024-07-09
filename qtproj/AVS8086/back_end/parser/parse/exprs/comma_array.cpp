#include "parser/parser.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/value.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_commaArray(const ExprPointer& comma)
{
    auto c = assert_dynamic_cast<Comma>(comma);
    auto max = ~(static_cast<size_t>(-1) << (m_currUnitSize << 3));
    bool err = false;

#define ERR if (!err) err = true; continue;

    for (const auto& e : c->expressions())
    {
        if (!expectExprAbleToEvaluate(e))
        {
            ERR;
        }
        if (e->is(Node::VALUE))
        {
            if (e->unitDataSize() > qMin(m_currUnitSize, 8))
            {
                addValueOverflowErrorInfo(e, max);
                ERR;
            }
        }
        else if (e->is(Node::DUP))
        {
#if 0
            auto d = assert_dynamic_cast<Dup>(e);
            d->resetUnitSize(m_currUnitSize);
#endif
        }
        else
        {
            Q_ASSERT_X(false, "Parser::parse_commaArray", "expr type error");
        }
    }

    if (err)
        return ExprPointer(nullptr);
    return ExprPointer(new CommaArray(c, m_currUnitSize));
}

/* ========================================================================== */

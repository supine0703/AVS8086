#include "parser/parser.h"
#include "ast/exprs/dup.h"
#include "ast/exprs/value.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_dup(const ExprPointer& left)
{
    auto p = currPrecedence();
    auto token = currToken();

    if (!expectPeekToken(true, Token::LPAREN))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::LPAREN});
        return ExprPointer(new Dup(token, left, parse_illegal(Token())));
    }

    auto right = parse_expression(p);

    if (right->is(Node::COMMA))
    {
        auto tmp = parse_commaArray(right);
        if (!tmp.isNull())
            right = tmp;
    }

    if (!expectExprAbleToEvaluate(right))
    {
        return ExprPointer(new Dup(token, left, right));
    }

    if (!left->is(Node::VALUE))
    {
        addExpectExprErrorInfo(left, {Node::VALUE});
        return ExprPointer(new Dup(token, left, right));
    }

    auto lve = assert_dynamic_cast<Value>(left);
    bool ok;
    auto count = lve->integer(&ok);
    if (!ok)
    {
        addExprCannotBeUsedAsIntegerErrorInfo(left);
    }

    return ExprPointer(new Dup(token, left, right, m_currUnitSize, count));
}

/* ========================================================================== */

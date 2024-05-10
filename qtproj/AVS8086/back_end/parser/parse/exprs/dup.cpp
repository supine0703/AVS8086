#include "parser/parser.h"
#include "ast/exprs/dup.h"
#include "ast/exprs/value.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

ExprPointer Parser::parse_dup(const ExprPointer& left)
{
    auto p = currPrecedence();
    auto token = currToken();

    if (!expectPeekToken(true, Token::LPAREN))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::LPAREN});
        return ExprPointer(new Dup(token, left, parse_illegal(Token()), 0));
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
        return ExprPointer(new Dup(token, left, right, 0));
    }

    auto lve = left.dynamicCast<Value>();
    if (lve.isNull())
    {
        addExpectExprErrorInfo(left, {Node::VALUE});
        return ExprPointer(new Dup(token, left, parse_illegal(Token()), 0));
    }

    bool ok;
    auto count = lve->integer(&ok);

    if (!ok)
    {
        addExprCanNotBeUsedAsIntegerErrorInfo(left);
    }

    return ExprPointer(new Dup(token, left, right, m_currUnitSize, count));
}

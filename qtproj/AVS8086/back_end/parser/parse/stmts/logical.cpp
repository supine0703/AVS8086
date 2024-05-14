#include "parser/parser.h"
#include "ast/stmts/multiple.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/address.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_logical_not()
{
    auto lxx = Multiple::NEW(currToken().type());
    Q_ASSERT(!lxx.isNull());

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        lxx->m_expr = parse_illegal(peekToken());
        addExpectExprErrorInfo(lxx->m_expr);
        return lxx;
    }

    auto e = parse_expression();
    lxx->m_expr = e;

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return lxx;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->m_exprs.size() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->m_exprs.size());
        return lxx;
    }

    // LXX reg, addr
    auto e1 = comma->m_exprs.at(0);
    auto e2 = comma->m_exprs.at(1);

    if (!e1->is(Node::REGISTER))
    {
        addExpectExprErrorInfo(e1, {Node::REGISTER});
        return lxx;
    }

    auto reg = assert_dynamic_cast<Register>(e1);

    if (!reg->m_token.is(Token::REG16))
    {
        addExpectTokenErrorInfo(reg->m_token, {Token::REG16});
        return lxx;
    }

    if (!e2->is(Node::ADDRESS))
    {
        addExpectExprErrorInfo(e2, {Node::ADDRESS});
        return lxx;
    }

    if (!expectExprAbleToEvaluate(e2))
    {
        return lxx;
    }

    lxx->m_codes.append(e2->bytes());
    lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;

    return lxx;
}

/* ========================================================================== */

StmtPointer Parser::parse_logical_bit()
{
    auto lxx = Multiple::NEW(currToken().type());
    Q_ASSERT(!lxx.isNull());

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        lxx->m_expr = parse_illegal(peekToken());
        addExpectExprErrorInfo(lxx->m_expr);
        return lxx;
    }

    auto e = parse_expression();
    lxx->m_expr = e;

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return lxx;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->m_exprs.size() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->m_exprs.size());
        return lxx;
    }

    // LXX reg, addr
    auto e1 = comma->m_exprs.at(0);
    auto e2 = comma->m_exprs.at(1);

    if (!e1->is(Node::REGISTER))
    {
        addExpectExprErrorInfo(e1, {Node::REGISTER});
        return lxx;
    }

    auto reg = assert_dynamic_cast<Register>(e1);

    if (!reg->m_token.is(Token::REG16))
    {
        addExpectTokenErrorInfo(reg->m_token, {Token::REG16});
        return lxx;
    }

    if (!e2->is(Node::ADDRESS))
    {
        addExpectExprErrorInfo(e2, {Node::ADDRESS});
        return lxx;
    }

    if (!expectExprAbleToEvaluate(e2))
    {
        return lxx;
    }

    lxx->m_codes.append(e2->bytes());
    lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;

    return lxx;
}

/* ========================================================================== */

StmtPointer Parser::parse_logical_shift()
{
    auto lxx = Multiple::NEW(currToken().type());
    Q_ASSERT(!lxx.isNull());

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        lxx->m_expr = parse_illegal(peekToken());
        addExpectExprErrorInfo(lxx->m_expr);
        return lxx;
    }

    auto e = parse_expression();
    lxx->m_expr = e;

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return lxx;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->m_exprs.size() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->m_exprs.size());
        return lxx;
    }

    // LXX reg, addr
    auto e1 = comma->m_exprs.at(0);
    auto e2 = comma->m_exprs.at(1);

    if (!e1->is(Node::REGISTER))
    {
        addExpectExprErrorInfo(e1, {Node::REGISTER});
        return lxx;
    }

    auto reg = assert_dynamic_cast<Register>(e1);

    if (!reg->m_token.is(Token::REG16))
    {
        addExpectTokenErrorInfo(reg->m_token, {Token::REG16});
        return lxx;
    }

    if (!e2->is(Node::ADDRESS))
    {
        addExpectExprErrorInfo(e2, {Node::ADDRESS});
        return lxx;
    }

    if (!expectExprAbleToEvaluate(e2))
    {
        return lxx;
    }

    lxx->m_codes.append(e2->bytes());
    lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;

    return lxx;
}

/* ========================================================================== */

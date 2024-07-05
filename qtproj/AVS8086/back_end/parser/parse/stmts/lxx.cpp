#include "parser/parser.h"
#include "ast/stmts/lxx.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/address.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_lxx()
{
    auto lxx = Lxx::Pointer(new Lxx(currToken().type()));

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        lxx->resetExpr(eol);
        addExpectExprErrorInfo(eol);
        return lxx;
    }

    auto e = parse_expression();
    lxx->resetExpr(e);

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return lxx;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->exprCount() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
        return lxx;
    }

    // LXX reg, addr
    auto e1 = comma->at(0);
    auto e2 = comma->at(1);

    if (!e1->is(Node::REGISTER))
    {
        addExpectExprErrorInfo(e1, {Node::REGISTER});
        return lxx;
    }

    auto reg = assert_dynamic_cast<Register>(e1);

    if (!reg->token().is(Token::REG16))
    {
        addExpectTokenErrorInfo(reg->token(), {Token::REG16});
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

    if (lxx->isLEA() && assert_dynamic_cast<Address>(e2)->is(Address::_16))
    { // 优化 lea reg, [imme] -> mov reg, imme
        lxx->set_reg_imme(reg->reg(), e2->bytes());
    }
    else
    {
        lxx->set_m_reg(e2->bytes(), reg->reg());
    }
#if 0
    lxx->m_codes.append(e2->bytes());
    lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;
#endif

    return lxx;
}

/* ========================================================================== */

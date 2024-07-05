#include "parser/parser.h"
#include "ast/exprs/infix.h"
#include "ast/exprs/address.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_colon(const ExprPointer& left)
{
    auto p = currPrecedence();
    auto token = currToken();

    Infix::Pointer infix(new Infix(token, left));

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        addExpectExprErrorInfo(parse_illegal(peekToken()));
        return infix;
    }

    if (!left->is(Node::REGISTER))
    {
        addExpectExprErrorInfo(left, {Node::REGISTER});
        return infix;
    }

    auto reg = assert_dynamic_cast<Register>(left);

    if (!reg->isSegReg())
    {
        addExpectTokenErrorInfo(reg->token(), {Token::REG16});
        // addInfo(Info::ERROR, left->pos(), "expect this register is segment");
        return infix;
    }

    auto right = parse_expression(p);
    infix->resetRight(right);

    if (!right->is(Node::ADDRESS))
    {
        addExpectExprErrorInfo(right, {Node::ADDRESS});
        return infix;
    }

    auto addr = assert_dynamic_cast<Address>(right);

    addr->changeSeg(reg);

    return addr;
}

/* ========================================================================== */

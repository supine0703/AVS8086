#include "parser/parser.h"
#include "ast/stmts/shift.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/value.h"
#include "ast/exprs/address.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_shift()
{
    auto shift = Shift::Pointer(new Shift(currToken().type()));

    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        shift->resetExpr(eol);
        addExpectExprErrorInfo(eol);
        return shift;
    }

    auto e = parse_expression();
    shift->resetExpr(e);

    if (!e->is(Node::COMMA))
    {
        addExpectExprErrorInfo(e, {Node::COMMA});
        return shift;
    }

    auto comma = assert_dynamic_cast<Comma>(e);

    if (comma->exprCount() != 2)
    {
        addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
        return shift;
    }

    // shift r/m, times (1/CL)
    auto rm = comma->at(0);
    auto times = comma->at(1);

//     if (!(rm->is(Node::REGISTER) || rm->is(Node::ADDRESS)))
//     {
//         addExpectExprErrorInfo(rm, {Node::REGISTER, Node::ADDRESS});
//         return shift;
//     }

//     if (times->is(Node::VALUE))
//     {
//         auto value = assert_dynamic_cast<Value>(times);

//         auto i = value->integer();
//         if (i != 1)
//         {
//             addValueOnlyBeErrorInfo(times, 1);
//             return shift;
//         }

//         shift->set_rm_one()
//     }
//     else if (times->is(Node::REGISTER))
//     {

//     }
//     else
//     {
//         addExpectExprErrorInfo(rm, {Node::REGISTER, Node::VALUE});
//         return shift;
//     }









//     if (!e1->is(Node::REGISTER))
//     {
//         addExpectExprErrorInfo(e1, {Node::REGISTER});
//         return shift;
//     }

//     auto reg = assert_dynamic_cast<Register>(e1);

//     if (!reg->token().is(Token::REG16))
//     {
//         addExpectTokenErrorInfo(reg->token(), {Token::REG16});
//         return shift;
//     }

//     if (!e2->is(Node::ADDRESS))
//     {
//         addExpectExprErrorInfo(e2, {Node::ADDRESS});
//         return shift;
//     }

//     if (!expectExprAbleToEvaluate(e2))
//     {
//         return shift;
//     }

//     if (shift->isLEA() && assert_dynamic_cast<Address>(e2)->is(Address::_16))
//     { // 优化 lea reg, [imme] -> mov reg, imme
//         shift->set_reg_imme(reg->reg(), e2->bytes());
//     }
//     else
//     {
//         lxx->set_m_reg(e2->bytes(), reg->reg());
//     }
// #if 0
//     lxx->m_codes.append(e2->bytes());
//     lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;
// #endif

    return shift;
}

/* ========================================================================== */

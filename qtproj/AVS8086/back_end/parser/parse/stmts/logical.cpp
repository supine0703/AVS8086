#include "parser/parser.h"
#include "ast/stmts/lxx.h"
#include "ast/exprs/comma.h"
#include "ast/exprs/address.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_logical_not(){}
// {
//     auto n = Lxx::NEW(currToken().type());
//     // Q_ASSERT(n->is(Node::NOT));

//     if (!expectPeekToken(false, Token::TOKEN_EOL))
//     {
//         n->m_expr = parse_illegal(peekToken());
//         addExpectExprErrorInfo(n->m_expr);
//         return n;
//     }

//     auto e = parse_expression();
//     n->m_expr = e;

//     if (!(e->is(Node::REGISTER) || e->is(Node::ADDRESS)))
//     {
//         addExpectExprErrorInfo(e, {Node::REGISTER, Node::ADDRESS});
//         return n;
//     }
//     if (e->token().is(Token::SREG))
//     {
//         addSRegCannotBeModifiedErrorInfo(e);
//         return n;
//     }

//     bool word = e->token().is(Token::REG16);

//     auto lxx = Lxx::NEW(currToken().type());
//     Q_ASSERT(!lxx.isNull());

//     if (!expectPeekToken(false, Token::TOKEN_EOL))
//     {
//         lxx->m_expr = parse_illegal(peekToken());
//         addExpectExprErrorInfo(lxx->m_expr);
//         return lxx;
//     }

//     // auto e = parse_expression();
//     lxx->m_expr = e;

//     if (!e->is(Node::COMMA))
//     {
//         addExpectExprErrorInfo(e, {Node::COMMA});
//         return lxx;
//     }

//     auto comma = assert_dynamic_cast<Comma>(e);

//     if (comma->exprCount() != 2)
//     {
//         addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
//         return lxx;
//     }

//     // LXX reg, addr
//     auto e1 = comma->at(0);
//     auto e2 = comma->at(1);

//     if (!e1->is(Node::REGISTER))
//     {
//         addExpectExprErrorInfo(e1, {Node::REGISTER});
//         return lxx;
//     }

//     auto reg = assert_dynamic_cast<Register>(e1);

//     if (!reg->token().is(Token::REG16))
//     {
//         addExpectTokenErrorInfo(reg->token(), {Token::REG16});
//         return lxx;
//     }

//     if (!e2->is(Node::ADDRESS))
//     {
//         addExpectExprErrorInfo(e2, {Node::ADDRESS});
//         return lxx;
//     }

//     if (!expectExprAbleToEvaluate(e2))
//     {
//         return lxx;
//     }

//     // lxx->m_codes.append(e2->bytes());
//     // lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;

//     return lxx;
// }

/* ========================================================================== */

StmtPointer Parser::parse_logical_bit(){}
// {
//     auto lxx = Lxx::NEW(currToken().type());
//     Q_ASSERT(!lxx.isNull());

//     if (!expectPeekToken(false, Token::TOKEN_EOL))
//     {
//         lxx->m_expr = parse_illegal(peekToken());
//         addExpectExprErrorInfo(lxx->m_expr);
//         return lxx;
//     }

//     auto e = parse_expression();
//     lxx->m_expr = e;

//     if (!e->is(Node::COMMA))
//     {
//         addExpectExprErrorInfo(e, {Node::COMMA});
//         return lxx;
//     }

//     auto comma = assert_dynamic_cast<Comma>(e);

//     if (comma->exprCount() != 2)
//     {
//         addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
//         return lxx;
//     }

//     // LXX reg, addr
//     auto e1 = comma->at(0);
//     auto e2 = comma->at(1);

//     if (!e1->is(Node::REGISTER))
//     {
//         addExpectExprErrorInfo(e1, {Node::REGISTER});
//         return lxx;
//     }

//     auto reg = assert_dynamic_cast<Register>(e1);

//     if (!reg->token().is(Token::REG16))
//     {
//         addExpectTokenErrorInfo(reg->token(), {Token::REG16});
//         return lxx;
//     }

//     if (!e2->is(Node::ADDRESS))
//     {
//         addExpectExprErrorInfo(e2, {Node::ADDRESS});
//         return lxx;
//     }

//     if (!expectExprAbleToEvaluate(e2))
//     {
//         return lxx;
//     }

//     // lxx->m_codes.append(e2->bytes());
//     // lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;

//     return lxx;
// }

/* ========================================================================== */

// {
//     auto lxx = Lxx::NEW(currToken().type());
//     Q_ASSERT(!lxx.isNull());

//     if (!expectPeekToken(false, Token::TOKEN_EOL))
//     {
//         lxx->m_expr = parse_illegal(peekToken());
//         addExpectExprErrorInfo(lxx->m_expr);
//         return lxx;
//     }

//     auto e = parse_expression();
//     lxx->m_expr = e;

//     if (!e->is(Node::COMMA))
//     {
//         addExpectExprErrorInfo(e, {Node::COMMA});
//         return lxx;
//     }

//     auto comma = assert_dynamic_cast<Comma>(e);

//     if (comma->exprCount() != 2)
//     {
//         addExpectCommaCountErrorInfo(comma, 2, comma->exprCount());
//         return lxx;
//     }

//     // LXX reg, addr
//     auto e1 = comma->at(0);
//     auto e2 = comma->at(1);

//     if (!e1->is(Node::REGISTER))
//     {
//         addExpectExprErrorInfo(e1, {Node::REGISTER});
//         return lxx;
//     }

//     auto reg = assert_dynamic_cast<Register>(e1);

//     if (!reg->token().is(Token::REG16))
//     {
//         addExpectTokenErrorInfo(reg->token(), {Token::REG16});
//         return lxx;
//     }

//     if (!e2->is(Node::ADDRESS))
//     {
//         addExpectExprErrorInfo(e2, {Node::ADDRESS});
//         return lxx;
//     }

//     if (!expectExprAbleToEvaluate(e2))
//     {
//         return lxx;
//     }

//     // lxx->m_codes.append(e2->bytes());
//     // lxx->m_codes[1] |= (reg->m_id & 0x07) << 3;

//     return lxx;
// }

/* ========================================================================== */

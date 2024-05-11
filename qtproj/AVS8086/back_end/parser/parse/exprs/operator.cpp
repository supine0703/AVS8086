#include "parser/parser.h"
#include "ast/exprs/operator.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

ExprPointer Parser::parse_operator()
{
    // 前缀 ~ + -
    // Token::BIT_NOT, Token::PLUS, Token::MINUS
    auto p = currPrecedence();
    auto token = currToken();
    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        addExpectExprErrorInfo(eol);
        return ExprPointer(new Operator(token, eol));
    }

    auto right = parse_expression(p);
    QSharedPointer<Operator> e(new Operator(token, right));

    if (!expectExprAbleToEvaluate(right))
    {
        return e;
    }

    auto ve = right.dynamicCast<Value>();
    if (ve.isNull())
    {
        addExpectExprErrorInfo(right, {Node::VALUE});
        return e;
    }

    bool ok;
    auto value = ve->integer(&ok);
    if (!ok)
    {
        addExprCanNotBeUsedAsIntegerErrorInfo(right);
        return e;
    }
    // 限制取值, 防止取反后数据膨胀
    auto limit = ~(static_cast<size_t>(-1) << (right->dataSize() << 3));

    switch (token.type())
    {
    case Token::BIT_NOT:
        value = (~value) & limit;
        break; // ~
    case Token::MINUS:
        value = (~value + 1) & limit;
        break; // -
    case Token::PLUS:
        break; // +
    default:
        Q_ASSERT_X(false, "Parser::parse_operator", "type of operator error");
    }

    auto tmp = Value(Token(), value);
    e->m_unitDataSize = tmp.unitDataSize();
    e->m_value = tmp.bytes();
    return e;
}

/* ========================================================================== */

ExprPointer Parser::parse_operator(const ExprPointer& left)
{
    auto p = currPrecedence();
    auto token = currToken();
    if (!expectPeekToken(false, Token::TOKEN_EOL))
    {
        auto eol = parse_illegal(peekToken());
        addExpectExprErrorInfo(eol);
        return ExprPointer(new Operator(token, left, eol));
    }

    auto right = parse_expression(p);
    QSharedPointer<Operator> e(new Operator(token, left, right));

    // 寄存器可以作为加号的表达式


    if (!(expectExprAbleToEvaluate(left) && expectExprAbleToEvaluate(right)))
    {
        return e;
    }

    auto lve = left.dynamicCast<Value>();
    if (lve.isNull())
    {
        addExpectExprErrorInfo(left, {Node::VALUE});
        return e;
    }
    auto rve = right.dynamicCast<Value>();
    if (rve.isNull())
    {
        addExpectExprErrorInfo(right, {Node::VALUE});
        return e;
    }

    bool ok;
    auto lv = lve->integer(&ok);
    if (!ok)
    {
        addExprCanNotBeUsedAsIntegerErrorInfo(left);
        return e;
    }
    auto rv = rve->integer(&ok);
    if (!ok)
    {
        addExprCanNotBeUsedAsIntegerErrorInfo(right);
        return e;
    }

    int size = qMax(left->dataSize(), right->dataSize());
    auto limit = ~(static_cast<size_t>(-1) << (size << 3));
    size_t value;
#define CASE_V(T, OP) case T: \
    value = (lv OP rv) & limit;
#define CASE_DIV_V(T, OP) case T: \
    if (rv == 0) { addExprDivideZeroErrorInfo(e); return e; } \
    value = (lv OP rv) & limit;
#define BOOL_V \
    if (!value) { value = static_cast<size_t>(-1); }

    switch (token.type())
    {
        CASE_V( Token::ASTERISK,    *  ); break;
        CASE_V( Token::PLUS,        +  ); break;
        CASE_V( Token::MINUS,       -  ); break;
        CASE_V( Token::LEFT_SHIFT,  << ); break;
        CASE_V( Token::RIGHT_SHIFT, >> ); break;
        CASE_V( Token::BIT_AND,     &  ); break;
        CASE_V( Token::BIT_XOR,     ^  ); break;
        CASE_V( Token::BIT_OR,      |  ); break;
        CASE_V( Token::LT,          <  ); BOOL_V; break;
        CASE_V( Token::GT,          >  ); BOOL_V; break;
        CASE_V( Token::LE,          <= ); BOOL_V; break;
        CASE_V( Token::GE,          >= ); BOOL_V; break;
        CASE_V( Token::EQ,          == ); BOOL_V; break;
        CASE_V( Token::NE,          != ); BOOL_V; break;
        CASE_DIV_V( Token::SLASH,   /  ); break;
        CASE_DIV_V( Token::MODULO,  %  ); break;
    default:
        Q_ASSERT_X(false, "Parser::parse_operator", "type of operator error");
    }

    auto tmp = Value(Token(), value);
    e->m_unitDataSize = tmp.unitDataSize();
    e->m_value = tmp.bytes();
    return e;
}

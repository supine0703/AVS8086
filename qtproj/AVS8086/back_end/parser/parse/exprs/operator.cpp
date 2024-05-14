#include "parser/parser.h"
#include "ast/exprs/operator.h"
#include "ast/exprs/register.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

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
        addExprCannotBeUsedAsIntegerErrorInfo(right);
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

    auto le = left;
    auto re = parse_expression(p);
    QSharedPointer<Operator> e(new Operator(token, le, re));

    auto parse_op = [this, &token](const QSharedPointer<Operator>& op) {
        const auto& l = op->m_left;
        const auto& r = op->m_right;
        if (!(expectExprAbleToEvaluate(l) && expectExprAbleToEvaluate(r)))
        {
            return ;
        }

        auto lve = l.dynamicCast<Value>();
        if (lve.isNull())
        {
            addExpectExprErrorInfo(l, {Node::VALUE});
            return ;
        }
        auto rve = r.dynamicCast<Value>();
        if (rve.isNull())
        {
            addExpectExprErrorInfo(r, {Node::VALUE});
            return ;
        }

        bool ok;
        auto lv = lve->integer(&ok);
        if (!ok)
        {
            addExprCannotBeUsedAsIntegerErrorInfo(l);
            return ;
        }
        auto rv = rve->integer(&ok);
        if (!ok)
        {
            addExprCannotBeUsedAsIntegerErrorInfo(r);
            return ;
        }

        int size = qMax(l->dataSize(), r->dataSize());
        auto limit = ~(static_cast<size_t>(-1) << (size << 3));
        size_t value;

#define BOOL_VZ if (!value) { value = static_cast<size_t>(-1); }
#define CASE_V(T, OP, F) case T: value = (lv OP rv) & limit; F; break;
#define CASE_DIV_V(T, OP) \
    case T: \
        if (rv == 0) { addExprDivideZeroErrorInfo(op); return ; } \
        value = (lv OP rv) & limit; \
        break;

        switch (token.type())
        {
            CASE_V( Token::ASTERISK,    *,          );
            CASE_V( Token::PLUS,        +,          );
            CASE_V( Token::MINUS,       -,          );
            CASE_V( Token::LEFT_SHIFT,  <<,         );
            CASE_V( Token::RIGHT_SHIFT, >>,         );
            CASE_V( Token::BIT_AND,     &,          );
            CASE_V( Token::BIT_XOR,     ^,          );
            CASE_V( Token::BIT_OR,      |,          );
            CASE_V( Token::LT,          <,  BOOL_VZ );
            CASE_V( Token::GT,          >,  BOOL_VZ );
            CASE_V( Token::LE,          <=, BOOL_VZ );
            CASE_V( Token::GE,          >=, BOOL_VZ );
            CASE_V( Token::EQ,          ==, BOOL_VZ );
            CASE_V( Token::NE,          !=, BOOL_VZ );
            CASE_DIV_V( Token::SLASH,   /           );
            CASE_DIV_V( Token::MODULO,  %           );
        default:
            Q_ASSERT_X(false, "Parser::parse_operator", "operator type error");
        }

        auto tmp = Value(Token(), value);
        op->m_unitDataSize = tmp.unitDataSize();
        op->m_value = tmp.bytes();
    };

    // 寄存器可以作为加号的表达式
    if (re->is(Node::REG_UNION) || re->is(Node::REGISTER))
    { // REG_UNION 优先级更高
        if (!le->is(Node::REG_UNION))
        {
            le.swap(re);
        }
    }

    if (le->is(Node::REG_UNION))
    { // right: reg union, reg, value
        if (!token.is(Token::PLUS))
        {
            addExpectTokenErrorInfo(token, {Token::PLUS});
            return e;
        }

        auto l_ru = assert_dynamic_cast<RegUnion>(le);

        if (re->is(Node::REG_UNION))
        {
            auto r_ru  = assert_dynamic_cast<RegUnion>(re);
            l_ru->unite(r_ru);
            if (l_ru->m_expr.isNull())
            {
                l_ru->resetExpr(r_ru->m_expr);
            }
            else if (!r_ru->m_expr.isNull())
            {
                QSharedPointer<Operator> op(
                    new Operator(token, l_ru->m_expr, r_ru->m_expr)
                );
                parse_op(op);
                if (expectExprAbleToEvaluate(op))
                {
                    l_ru->resetExpr(op);
                }
            }
        }
        else if (re->is(Node::REGISTER))
        {
            l_ru->insert(assert_dynamic_cast<Register>(re));
        }
        else
        {
            auto rve = re.dynamicCast<Value>();
            if (rve.isNull())
            { // 不能仅判断可求值性, 必须是 value 或 operator
                addExpectExprErrorInfo(
                    re, {Node::VALUE, Node::REGISTER, Node::REG_UNION}
                );
                return e;
            }
            if (l_ru->m_expr.isNull())
            {
                l_ru->resetExpr(re);
            }
            else
            {
                QSharedPointer<Operator> op(
                    new Operator(token, l_ru->m_expr, re)
                );
                l_ru->resetExpr((parse_op(op), op));
            }
            return l_ru;
        }
        return l_ru;
    }
    else if (le->is(Node::REGISTER))
    { // right: reg, value
        if (!token.is(Token::PLUS))
        {
            addExpectTokenErrorInfo(token, {Token::PLUS});
            return e;
        }

        if (re->is(Node::REGISTER))
        {
            QSharedPointer<RegUnion> ru(new RegUnion(token));
            ru->insert(assert_dynamic_cast<Register>(re));
            ru->insert(assert_dynamic_cast<Register>(le));
            return ru;
        }
        else
        {
            auto rve = re.dynamicCast<Value>();
            if (rve.isNull())
            { // 不能仅判断可求值性, 必须是 value 或 operator
                addExpectExprErrorInfo(re, {Node::VALUE, Node::REGISTER});
                return e;
            }
            if (!expectExprAbleToEvaluate(re))
            {
                return e;
            }
            QSharedPointer<RegUnion> ru(new RegUnion(token));
            ru->resetExpr(re);
            ru->insert(assert_dynamic_cast<Register>(le));
            return ru;
        }
    }

    return (parse_op(e), e); // else 不包含寄存器
}

/* ========================================================================== */

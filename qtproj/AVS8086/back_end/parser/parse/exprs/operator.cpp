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

    if (!right->is(Node::VALUE))
    {
        addExpectExprErrorInfo(right, {Node::VALUE});
        return e;
    }

    auto ve = assert_dynamic_cast<Value>(right);

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

    e.reset(new Operator(*e, value));
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
    QSharedPointer<Operator> op_e(new Operator(token, le, re));

    auto parse_op = [this, &token](QSharedPointer<Operator>& op) {
        auto l = op->left();
        auto r = op->right();

        if (!(expectExprAbleToEvaluate(l) && expectExprAbleToEvaluate(r)))
        {
            return;
        }

        if (!l->is(Node::VALUE))
        {
            addExpectExprErrorInfo(l, {Node::VALUE});
            return;
        }
        if (!r->is(Node::VALUE))
        {
            addExpectExprErrorInfo(r, {Node::VALUE});
            return;
        }

        auto lve = assert_dynamic_cast<Value>(l);
        auto rve = assert_dynamic_cast<Value>(r);

        bool ok;
        auto lv = lve->integer(&ok);
        if (!ok)
        {
            addExprCannotBeUsedAsIntegerErrorInfo(l);
            return;
        }
        auto rv = rve->integer(&ok);
        if (!ok)
        {
            addExprCannotBeUsedAsIntegerErrorInfo(r);
            return;
        }

        int size = qMax(l->dataSize(), r->dataSize());
        auto limit = ~(static_cast<size_t>(-1) << (size << 3));
        size_t value;

#define BOOL_VZ if (!value) { value = static_cast<size_t>(-1); }
#define CASE_V(T, OP, F) case T: value = (lv OP rv) & limit; F; break;
#define CASE_DIV_V(T, OP) \
    case T: \
        if (rv == 0) { addExprDivideZeroErrorInfo(op); return; } \
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

        op.reset(new Operator(*op, value));
    };

    // 寄存器可以作为加号的表达式
    if ((re->is(Node::REG_UNION) || re->is(Node::REGISTER))
        && !le->is(Node::REG_UNION))
    { // REG_UNION 优先级更高
        le.swap(re);
    }

    if (le->is(Node::REG_UNION))
    { // right: reg union, reg, value
        if (!token.is(Token::PLUS))
        {
            addExpectTokenErrorInfo(token, {Token::PLUS});
            return op_e;
        }

        auto l_ru = assert_dynamic_cast<RegUnion>(le);

        if (re->is(Node::REG_UNION))
        {
            auto r_ru  = assert_dynamic_cast<RegUnion>(re);
            l_ru->unite(r_ru);
            if (!l_ru->hasExpr())
            {
                l_ru->resetExpr(r_ru->expr());
            }
            else if (r_ru->hasExpr())
            {
                QSharedPointer<Operator> op(
                    new Operator(token, l_ru->expr(), r_ru->expr())
                );
                if (expectExprAbleToEvaluate((parse_op(op), op)))
                {
                    l_ru->resetExpr(op);
                }
            }
        }
        else if (re->is(Node::REGISTER))
        {
            l_ru->insert(assert_dynamic_cast<Register>(re));
        }
        else if (re->is(Node::VALUE))
        {
            if (l_ru->hasExpr())
            {
                QSharedPointer<Operator> op(
                    new Operator(token, l_ru->expr(), re)
                );
                l_ru->resetExpr((parse_op(op), op));
            }
            else
            {
                l_ru->resetExpr(re);
            }
        }
        else
        {
            addExpectExprErrorInfo(
                re, {Node::VALUE, Node::REGISTER, Node::REG_UNION}
            );
            return op_e;
        }
        return le;
    }
    else if (le->is(Node::REGISTER))
    { // right: reg, value
        if (!token.is(Token::PLUS))
        {
            addExpectTokenErrorInfo(token, {Token::PLUS});
            return op_e;
        }

        if (re->is(Node::REGISTER))
        {
            QSharedPointer<RegUnion> ru(new RegUnion(token));
            ru->insert(assert_dynamic_cast<Register>(re));
            ru->insert(assert_dynamic_cast<Register>(le));
            return ru;
        }
        else if (re->is(Node::VALUE))
        {
            if (!expectExprAbleToEvaluate(re))
            {
                return op_e;
            }
            QSharedPointer<RegUnion> ru(new RegUnion(token));
            ru->resetExpr(re);
            ru->insert(assert_dynamic_cast<Register>(le));
            return ru;
        }
        else
        {
            addExpectExprErrorInfo(re, {Node::VALUE, Node::REGISTER});
            return op_e;
        }
    }

    return (parse_op(op_e), op_e); // else 不包含寄存器
}

/* ========================================================================== */

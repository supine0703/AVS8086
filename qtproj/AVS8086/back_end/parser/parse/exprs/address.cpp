#include "parser/parser.h"
#include "ast/exprs/value.h"
#include "ast/exprs/address.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_address()
{
    auto token = currToken();

    nextToken();
    if (currToken().is(Token::RSQUARE))
    {
        addInfo(
            Info::ERROR, token.pos() + currToken().pos(),
            "empty in '[]'"
        );
        return parse_illegal(currToken());
    }

    auto e = parse_expression();

    if (peekToken().is(Token::TOKEN_EOL))
    {
        addInfo(
            Info::ERROR, token.pos() + currToken().pos(),
            "'[' is not close on this line"
        );
    }
    else if (!expectPeekToken(true, Token::RSQUARE))
    {
        addExpectTokenErrorInfo(peekToken(), {Token::RSQUARE});
    }

    QSharedPointer<Address> addr(new Address(token, e));

    QList<uint8_t> key;
    QByteArray vc;

    if (e->is(Node::REG_UNION))
    {
        auto ru = assert_dynamic_cast<RegUnion>(e);
        key.append(ru->m_regs.keys());
        if (!ru->m_expr.isNull())
        {
            auto v = assert_dynamic_cast<Value>(ru->m_expr);
            bool ok;
            auto i = v->integer(&ok);
            if (!ok || i > 0xffff)
            {
                addExprVOverflowErrorInfo(ru->m_expr, 0xffff);
            }
            else if (i <= 0xff)
            {
                key.append(8);
                vc.append(static_cast<char>(i & 0xff));
            }
            else
            {
                key.append(16);
                vc.append(static_cast<char>(i & 0xff));
                vc.append(static_cast<char>((i >> 8) & 0xff));
            }
        }
    }
    else if (e->is(Node::REGISTER))
    {
        key.append(assert_dynamic_cast<Register>(e)->m_id);
    }
    else
    {
        auto v = e.dynamicCast<Value>();
        if (!v.isNull())
        {
            if (!expectExprAbleToEvaluate(e))
            {
                return addr;
            }
            bool ok;
            auto i = v->integer(&ok);
            if (!ok || i > 0xffff)
            {
                addExprVOverflowErrorInfo(e, 0xffff);
            }
            else
            {
                key.append(16);
                vc.append(static_cast<char>(i & 0xff));
                vc.append(static_cast<char>((i >> 8) & 0xff));
            }
        }
        else
        {
            addExpectExprErrorInfo(
                v, {Node::VALUE, Node::REGISTER, Node::REG_UNION}
            );
            return addr;
        }
    }

    if (addr->setCode(key))
    {
        addr->m_codes.append(vc);
    }
    else
    {
        addInfo(Info::ERROR, e->pos(), "this expression cannot be addressed");
    }

    return addr;
}

/* ========================================================================== */

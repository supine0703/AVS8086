#include "parser/parser.h"
#include "ast/stmts/jmp.h"
#include "ast/stmts/jmp_address.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

StmtPointer Parser::parse_jmp()
{
    if (expectPeekToken(true, {Token::WORD, Token::DWORD}))
    {
        bool d = currToken().is(Token::DWORD);
        if (!expectPeekToken(true, Token::PTR))
        {
            addExpectTokenErrorInfo(peekToken(), {Token::PTR});
        }
        nextToken();
        auto e = parse_expression();
        JmpAddress::Pointer ja(new JmpAddress(e));
        if (e->is(Node::ADDRESS) || e->is(Node::REGISTER))
        {
            ja->setCode(e->bytes(), d);
        }
        else
        {
            addExpectExprErrorInfo(e, {Node::ADDRESS, Node::REGISTER});
        }
        return ja;
    }

    auto token = currToken();
    StmtPointer s;
    if (expectPeekToken(true, Token::SHORT))
    {
        s.reset(new Jmp(token, peekToken(), 2));
    }
    else if (expectPeekToken(true, {Token::NEAR, Token::FAR}))
    {
        int size = currToken().is(Token::NEAR) ? 3 : 5;
        if (!expectPeekToken(true, Token::PTR))
        {
            addExpectTokenErrorInfo(peekToken(), {Token::PTR});
        }
        s.reset(new Jmp(token, peekToken(), size));
    }
    else
    {
        s.reset(new Jmp(token, peekToken()));
    }

    nextToken(); // 检错包含在 callId(s) 中了
    callId(s);
    return s;
}

/* ========================================================================== */

StmtPointer Parser::parse_jx()
{
#if 0
    auto jx = Jx::NEW(currToken().type());
    Q_ASSERT(!jx.isNull());
    jx->m_id = peekToken();
    jx->m_pos = currToken().pos() + peekToken().pos();
#endif
    StmtPointer jx(new Jx(currToken(), peekToken()));
    nextToken();
    callId(jx);
    return jx;
}

/* ========================================================================== */

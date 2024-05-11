#include "parser/parser.h"
#include "ast/stmts/define.h"
#include "ast/exprs/value.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_define()
{
    if (peekToken().is(Token::ALLOCATE))
    {
        return parse_allocate();
    }

    QSharedPointer<Define> d(new Define(*currToken(), peekToken()));
    defineId(d);

    if (expectPeekToken(true, Token::COLON))
    { // label:
        if (expectPeekToken(false, Token::TOKEN_EOL))
            d->m_stmt = parse_statement();
    }
    // TODO
    else if (expectPeekToken(true, Token::SEGMENT))
    { // label segment ...
        addNotYetSupportErrorInfo();
    }
    else if (expectPeekToken(true, Token::EQU))
    { // constant equ ...
        addNotYetSupportErrorInfo();
    }
    else if (expectPeekToken(true, Token::ASSIGN))
    { // variable = ...
        addNotYetSupportErrorInfo();
    }
    else
    {
        Q_ASSERT_X(false, "Parser::parse_define", "type of define is error");
    }
    return d;
}

#if 0
    StmtPointer s(nullptr);
    int len = peekToken().endColumn() - col;
    QString id = *currToken();

    if (expectPeekToken(true, {Token::COLON, Token::SEGMENT, Token::ALLOCATE}))
    {
        switch (currToken().type())
        {
        case Token::COLON:
            // label
            if (expectPeekToken(false, Token::TOKEN_EOL))
                s = parse_statement();
            break;
        case Token::SEGMENT:
            // segment
            // TODO: at
            break;
        case Token::ALLOCATE:
            // variable
            if (expectPeekToken(false, Token::TOKEN_EOL))
            {
                // QSharedPointer<Variable> var(new Variable);
                // var->m_expr = parse_expression();
                // s = var;
                // if (!var->m_expr->is(Node::COMMA))
                // {
                //     // addErrorInfo(row, col, len, "");
                // }
            }
            else
            {
                // addErrorInfo(row, col, len, "can not define null variable");
            }
            break;
        default:
            break;
        }

        // bool exist = !(m_idList->append(
        //     Identifier(id , Identifier::LABEL, m_offset, m_segment)
        // ));
        // if (exist)
        // {
        //     addErrorInfo(row, col, len, "redefined identifier: " + id);
        // }

        if (expectPeekToken(false, Token::TOKEN_EOL))
            return parse_statement();
    }
    return s;
#endif

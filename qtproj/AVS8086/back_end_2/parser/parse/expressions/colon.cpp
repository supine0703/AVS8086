#include "parser/parser.h"
#include "ast/nodes/colon.hpp"
#include "ast/nodes/register.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression>
Parser::parse_colon(const QSharedPointer<ast::Expression>& left)
{
    QSharedPointer<Colon> c(new Colon(left));

    int row = currToken().row();
    int col = currToken().column();
    int len = 1;
    int sreg_code;
    if (left->is(Node::NODE_LABEL))
    {

    }
    else if (left->is(Node::NODE_REGISTER))
    {
        auto reg = qSharedPointerDynamicCast<Register>(left);
        auto s = reg->value().toString();

        len += col - reg->m_token.column();
        col = reg->m_token.column();

        int index = Token::findRegisters(s);

        if (15 < index && index < 20)
        {
            auto precedence = currTokenPrecedence();
            // sreg_code = index - 16;
            if (expectPeekToken(Token::TOKEN_LSQUARE))
            {
                auto e = parse_expression(precedence);
                c->m_right = e;
            }
            else
            {
                addExpectPeekTokenErrorInfo(Token::TOKEN_LSQUARE);
            }
        }
        else
        {
            addErrorInfo(
                row, col, len, "':' it is not sreg, can not match with: " + s
            );
        }
    }
    else
    {
        addErrorInfo(row, col, len, "':' can not match here");
    }

    return c;
}

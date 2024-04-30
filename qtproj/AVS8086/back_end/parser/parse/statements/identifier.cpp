#include "parser/parser.h"
#include "ast/nodes/identifier.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

StmtPointer Parser::parse_identifier()
{
    StmtPointer s(nullptr);

    int row = currToken().row();
    int col = currToken().column();
    int len = peekToken().endColumn() - col;
    QString id = currToken().literal();

    if (expectPeekToken({Token::COLON, Token::SEGMENT, Token::DEFINE}))
    {
        switch (currToken().type())
        {
        case Token::COLON:
            // label
            if (expectPeekTokenNot(Token::TOKEN_EOF))
                s = parse_statement();
            break;
        case Token::SEGMENT:
            // segment
            // TODO: at
            break;
        case Token::DEFINE:
            // variable
            if (expectPeekTokenNot(Token::TOKEN_EOF))
            {
                QSharedPointer<Variable> var(new Variable);
                var->m_expr = parse_expression();
                s = var;
                if (!var->m_expr->is(Node::COMMA))
                {
                    addErrorInfo(row, col, len, "");
                }
            }
            else
            {
                addErrorInfo(row, col, len, "can not define null variable");
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
        if (expectPeekTokenNot(Token::TOKEN_EOF))
            return parse_statement();
    }
    return s;
}

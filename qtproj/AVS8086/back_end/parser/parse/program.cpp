#include "parser/parser.h"
#include "ast/nodes/program.hpp"
#include "ast/nodes/multiple_statement.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Program> Parser::parse_program()
{
    QSharedPointer<Program> s(new Program(m_lexer->fileName()));
    while (!currToken().is(Token::TOKEN_EOF))
    {
        auto stmt = parse_statement();
        if (!stmt.isNull())
        {
            if (stmt->is(Node::NODE_MULTIPLE_STATEMENT))
            {
                s->m_statements.append(
                    qSharedPointerDynamicCast<MultipleStatement>
                    (stmt)->statements()
                );
            }
            else
            {
                s->m_statements.append(stmt);
            }
        }
        nextToken();
        if (currToken().is(Token::TOKEN_EOF))
        {
            nextToken();
        }
        else if (!isError())
        {
            const auto& t = currToken();
            addWarningInfo(
                t.row(), t.column(), t.literal().length(),
                "should only one statement on one line"
            );
        }
    }
    return s;
}

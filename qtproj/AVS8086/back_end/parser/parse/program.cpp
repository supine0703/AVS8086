#include "parser/parser.h"
#include "ast/nodes/program.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

QSharedPointer<Program> Parser::parse_program()
{
    QSharedPointer<Program> s(new Program(m_lexer->fileName()));
    while (!currToken().is(Token::TOKEN_EOF))
    {
        int errC = m_errorInfos.length();
        auto stmt = parse_statement();

        bool lineBreak = expectPeekToken(Token::TOKEN_EOF, false);

        if (!stmt.isNull())
        {
            s->m_statements.append(stmt);
            if (!lineBreak && errC == m_errorInfos.length())
            {
                const auto& t = peekToken();
                // TODO: translate
                addWarningInfo(
                    t.row(), t.column(), t.length(),
                    "should only one statement on one line"
                    );
            }
        }
        nextToken();
    }
    m_segment.clear();
    m_offset = 0;
    return s;
}

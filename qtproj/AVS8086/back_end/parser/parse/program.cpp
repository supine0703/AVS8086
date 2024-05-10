#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;

QSharedPointer<Program> Parser::parse_program()
{
    QSharedPointer<Program> p(new Program(m_lexer->file()));
    while (expectPeekToken(false, Token::TOKEN_EOF))
    {
        auto stmt = parse_statement();

        if (!expectPeekToken(true, Token::TOKEN_EOL))
            ;

        if (!stmt.isNull())
        {
            stmt->addIn(stmt, p->m_stmts);
#if 0
            int row = currToken().row();
            auto it = m_infos.lowerBound({Info::ERROR, {row, 0, 0}, ""});
            bool err = ((it != m_infos.end()) && (it->position().row() == row));
            if (!lineEnd && !err)
            {
                // TODO: translate
                addInfo(Info::WARNING, peekToken().position(),
                        "should only one statement on one line");
            }
#endif
        }
        m_currUnitSize = 0;
    }
    parse_idTable();
    p->m_err = isError();
    return p;
}

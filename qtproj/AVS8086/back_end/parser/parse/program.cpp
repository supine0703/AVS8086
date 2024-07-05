#include "parser/parser.h"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

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
            int i = p->m_stmts.size();
            stmt->addIn(stmt, p->m_stmts);
            while (i < p->m_stmts.size())
            {
                m_currOffset += p->m_stmts.at(i++)->codeSize();
            }
#if 0
            int row = currToken().row();
            auto it = m_infos.lowerBound({Info::ERROR, {row, 0, 0}, ""});
            bool err = ((it != m_infos.end()) && (it->pos().row() == row));
            if (!lineEnd && !err)
            {
                // TODO: translate
                addInfo(Info::WARNING, peekToken().pos(),
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

/* ========================================================================== */

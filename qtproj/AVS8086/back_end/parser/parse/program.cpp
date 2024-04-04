#include "parser/parser.h"
#include "ast/nodes/program.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Program> Parser::parse_program()
{
    QSharedPointer<Program> s(new Program(m_lexer->fileName()));
    while (!checkCurrToken(Token::TOKEN_EOF))
    {
        auto stmt = parse_statement();
        if (stmt != nullptr)
        {
            s->m_statements.append(stmt);
        }
        nextToken();
        if (checkCurrToken(Token::TOKEN_EOF))
            nextToken();
        else
        {
            const auto& t = currToken();
            addWarningInfo(t.row(), t.column(), t.literal().length(),
                           "should only one statement on one line");
        }
    }
    return s;
}

#include "parser/parser.h"
#include "ast/node/program.hpp"


QSharedPointer<Program> Parser::parse_program()
{
    QSharedPointer<Program> s(new Program(currToken()));
    nextToken();
    while (!checkCurrToken(Token::TOKEN_EOF))
    {
        auto stmt = parse_statement();
        if (stmt != nullptr)
        {
            s->append(stmt);
        }
        nextToken();
    }
    return s;
}

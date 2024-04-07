#include "parser/parser.h"
#include "ast/nodes/single.hpp"

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Statement> Parser::parse_single()
{
    QSharedPointer<Statement> s(new Single(currToken()));
    s->m_token = currToken();
    nextToken();
    return s;
}

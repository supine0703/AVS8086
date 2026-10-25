#include "ast/nodes/single.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::parser;

QSharedPointer<Statement> Parser::parse_single()
{
    QSharedPointer<Statement> s(new Single(currToken()));
    s->m_token = currToken();
    nextToken();
    return s;
}

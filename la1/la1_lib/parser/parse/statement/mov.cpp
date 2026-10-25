#include "ast/nodes/mov.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::token;
using namespace lzl::a1::parser;

QSharedPointer<Statement> Parser::parse_mov()
{
    int row = currToken().row();
    int col = currToken().column();

    nextToken();
    auto e = parse_expression(LOWEST);

    int len = currToken().column() + currToken().literal().length() + 1;

    if (!e->is(Node::NODE_COMMA))
    {
        addErrorInfo(
            row,
            col,
            len,
            QString("MOV member is false: %1, need: %2")
                .arg(e->typeName(), Node::nodeTypeName(Node::NODE_COMMA))
        );
    }

    return QSharedPointer<Statement>(new MOV(e));
}

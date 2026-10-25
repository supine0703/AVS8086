#include "ast/nodes/float.hpp"

#include "parser/parser.h"


using namespace lzl::a1::ast;
using namespace lzl::a1::parser;

QSharedPointer<Expression> Parser::parse_float()
{
    const auto& token = currToken();
    addErrorInfo(
        token.row(),
        token.column(),
        token.literal().length(),
        QString("not supported, unable to process '%1: %2'").arg(token.typeName(), token.literal())
    );
    return QSharedPointer<Expression>(new Float(token));
}

#include "parser/parser.h"
#include "ast/nodes/illegal.hpp"
using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_illegal()
{
    // const auto& token = currToken();
    // addErrorInfo(
    //     token.row(), token.column(), token.literal().length(),
    //     QString("at present, can not deal with '%1: %2'")
    //         .arg(token.typeName(), token.literal())
    // );
    return QSharedPointer<Expression>(new Illegal(currToken()));
}

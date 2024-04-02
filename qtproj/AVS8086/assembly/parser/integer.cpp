#include "parser/parser.h"
#include "ast/node/integer.hpp"


QSharedPointer<Expression> Parser::parse_integer()
{
    QSharedPointer<Integer> e(new Integer(currToken()));
    e->setValue(currToken().literal().toInt());
    return e;
}

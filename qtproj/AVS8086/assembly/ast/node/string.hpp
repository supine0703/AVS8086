#ifndef STRING_HPP
#define STRING_HPP

#include "ast/node.h"

class String : public Expression
{
public:
    String() : Expression(NODE_STRING) { }
    ~String() { }


public:
    QString m_value;
};

#endif // STRING_H

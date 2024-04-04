#ifndef INFIX_HPP
#define INFIX_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Infix : public Expression
{
public:
    Infix(
        const QSharedPointer<Expression>& left,
        const QSharedPointer<Expression>& right,
        const token::Token& token = token::Token()
        )
        : Expression(NODE_INFIX)
        , m_left(left)
        , m_right(right)
        , m_token_type(token.type())
        , m_op(token.literal())
    {
        if (m_left.isNull() || m_right.isNull())
            goError();
    }
    ~Infix() { }
    
    QStringList traversal(int depth) const override
    {
        if (isError())
            return {
                QString("%1| %2: member pointer is null!")
                    .arg(QString(depth * 4, '-'), typeName())
            };
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), m_op
        ));
        depth++;
        QStringList s;
        s = m_left->traversal(depth);
        s[0].replace("|", "| l -");
        info.append(s);

        s = m_right->traversal(depth);
        s[0].replace("|", "| r -");
        info.append(s);

        return info;
    }


private:
    token::Token::Type m_token_type;
    QString m_op;
    QSharedPointer<Expression> m_left;
    QSharedPointer<Expression> m_right;
};

} // namespace avs8086::ast

#endif // INFIX_HPP

#ifndef INFIX_HPP
#define INFIX_HPP

#include "ast/node.h"
#include "token/token.h"
#include <QSharedPointer>

class Infix : public Expression
{
public:
    Infix(const Token& token = Token())
        : Expression(NODE_INFIX), m_token(token)
    { }
    ~Infix() { }
    
    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(), m_token.literal()
        ));
        depth++;
        auto s = m_left->traversal(depth);
        s[0].replace("|", "| l -");
        info.append(s);
        s = m_right->traversal(depth);
        s[0].replace("|", "| r -");
        info.append(s);
        return info;
    }

    void setOperator(const QString& op)
    { m_operator = op; }

    void setLeft(const QSharedPointer<Expression>& left)
    { m_left = left; }

    void setRight(const QSharedPointer<Expression>& right)
    { m_right = right; }


private:
    Token m_token;
    QString m_operator;
    QSharedPointer<Expression> m_left;
    QSharedPointer<Expression> m_right;
};

#endif // INFIX_HPP

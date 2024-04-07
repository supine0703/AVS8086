#ifndef INFIX_HPP
#define INFIX_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Infix : public Expression
{
public:
    Infix(const QSharedPointer<Expression>& left,
          const QSharedPointer<Expression>& right,
          const token::Token& token = token::Token())
        : Expression(NODE_INFIX)
        , m_left(left)
        , m_right(right)
        , m_op(token.literal())
    {
        m_token = token;
        static const QStringList op({
            "*", "/", "%", "+", "-", "&", "^", "|", "<<", ">>",
            "=", ">", ">=", "<", "<=", "==", "!="
        }); // 17
        if (m_left->isError() || m_right->isError())
        {
            goError();
            return;
        }

        if (left->valueType() < right->valueType())
            m_valueType =  left->valueType();
        else
            m_valueType =  right->valueType();

        if (m_valueType == Expression::REGISTER)
        {
            return;
        }

        int index = op.indexOf(m_op);
        switch (index)
        {
        case 0:
            m_value = left->value() * right->value();
            break;
        case 1:
            m_value = left->value() / right->value();
            break;
        case 2:
            m_value = left->value() % right->value();
            break;
        case 3:
            m_value = left->value() + right->value();
            break;
        case 4:
            m_value = left->value() - right->value();
            break;
        case 5:
            m_value = left->value() & right->value();
            break;
        case 6:
            m_value = left->value() ^ right->value();
            break;
        case 7:
            m_value = left->value() | right->value();
            break;
        case 8:
            m_value = left->value() << right->value();
            break;
        case 9:
            m_value = left->value() >> right->value();
            break;
        case 10:
            // =
            break;
        case 11:
            m_value = left->value() > right->value();
            break;
        case 12:
            m_value = left->value() >= right->value();
            break;
        case 13:
            m_value = left->value() < right->value();
            break;
        case 14:
            m_value = left->value() <= right->value();
            break;
        case 15:
            m_value = left->value() == right->value();
            break;
        case 16:
            m_value = left->value() != right->value();
            break;
        default:
            break;
        }
    }
    ~Infix() { }
    
    QStringList traversal(int depth) const override
    {
        QStringList info;
        if (m_valueType == INTEGER)
        {
            info.append(QString("%1| %2: %3").arg(
                QString(depth * 4, '-'), typeName(),
                m_value.toHex(' ')
            ));
        }
        else if (m_valueType == STRING)
        {
            info.append(QString("%1| %2: %3 : %4(%5)").arg(
                QString(depth * 4, '-'), typeName(), m_op,
                m_value.toHex(' '), m_value.toString()
            ));
        }
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

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["op"] = m_op;
        js["hexValue"] = m_value.toHex(' ');
        js["strValue"] = m_value.toString();
        js["left"] = m_left->json();
        js["right"] = m_right->json();
        return js;
    }

    QString op() const { return m_op; }

    QSharedPointer<Expression> left() const { return m_left; }

    QSharedPointer<Expression> right() const { return m_right; }


private:
    QString m_op;
    QSharedPointer<Expression> m_left;
    QSharedPointer<Expression> m_right;
};

} // namespace avs8086::ast

#endif // INFIX_HPP

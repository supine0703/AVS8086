#ifndef WELL_HPP
#define WELL_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Well : public Statement
{
public:
    Well(const QList<token::Token>& tokens)
        : Statement(NODE_WELL)
        , m_keyType(tokens.at(0).type())
        , m_key(tokens.at(0).literal().toUpper())
    {
        if (m_keyType == token::Token::TOKEN_MAKE_ && tokens.length() == 1)
        {
            auto s = m_key.split('_');
            m_op = "_";
            m_key = s.at(0);
            m_value = s.at(1);
        }
        else if ((m_keyType == token::Token::TOKEN_REGISTER
                  || m_keyType == token::Token::TOKEN_LOAD_)
                 && tokens.at(1).literal() == "="
                 && tokens.last().is(token::Token::TOKEN_INTEGER)
                 && (tokens.length() == 3 || tokens.length() == 4))
        {
            m_op = "=";
            m_value = QString::number(
                token::Token::textToInt(tokens.last().literal())
            );
            if (tokens.length() == 4)
            {
                m_value.push_front(tokens.at(3).literal());
                if (tokens.at(3).literal() != "-"
                    || tokens.at(3).literal() != "+")
                    goError();
            }
        }
        else
        {
            int space =
                tokens.at(0).column() + tokens.at(0).literal().length();
            for (int i = 1; i < tokens.length(); i++)
            {
                const auto& t = tokens.at(i);
                m_key.append(QString(t.column() - space, ' '));
                m_key.append(t.literal());
                space = t.column() + t.literal().length();
            }
            goError();
        }
    }
    ~Well() { }

    QStringList traversal(int depth) const override
    {
        return {
            QString("%1| '%2'Statement:%4 %3").arg(
                QString(depth * 4, '-'), typeName(),
                isError() ? m_key : m_key + m_op + m_value,
                isError() ? " illegal instruction:" : ""
            )
        };
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["op"] = m_op;
        js["Statement"] = isError() ? m_key : m_key + m_op + m_value;
        return js;
    }

    QString op() const { return m_op; }
    QString key() const { return m_key; }
    QString value() const { return m_value; }
    token::Token::Type valueType() const { return m_keyType; }


private:
    QString m_op;
    QString m_key;
    QString m_value;
    token::Token::Type m_keyType;
};

} // namespace avs8086::ast

#endif // WELL_HPP

#ifndef REGISTER_HPP
#define REGISTER_HPP

#ifndef INTEGER_HPP
#define INTEGER_HPP

#include "ast/node.h"
#include "token/token.h"

namespace avs8086::ast {

class Register : public Expression
{
public:
    enum RegType {
        SEGMENT,
        R_8BIT,
        R_16BIT,
    };

public:
    Register(const token::Token& token)
        : Expression(NODE_REGISTER)
    {
        m_token = token;
        m_valueType = REGISTER;
        QString r = token.literal().toUpper();
        m_value = r;
        if (r == "ES" || r == "CS" || r == "SS" || r == "DS")
            m_regType = SEGMENT;
        else if (r == "AL" || r == "CL" || r == "DL" || r == "BL"
                 || r == "AH" || r == "CH" || r == "DH" || r == "BH")
            m_regType = R_8BIT;
        else
            m_regType = R_16BIT;
    }
    ~Register() { }

    QStringList traversal(int depth) const override
    {
        QStringList info;
        info.append(QString("%1| %2: %3").arg(
            QString(depth * 4, '-'), typeName(),
            m_value.toString()
            ));
        return info;
    }

    QJsonObject json() const override
    {
        QJsonObject js;
        js["type"] = typeName();
        js["value"] = m_value.toString();
        return js;
    }

    RegType regType() const { return m_regType; }


private:
    RegType m_regType;
};

} // namespace avs8086::ast

#endif // INTEGER_HPP


#endif // REGISTER_HPP

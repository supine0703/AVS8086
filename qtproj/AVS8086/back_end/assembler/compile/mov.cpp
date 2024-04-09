#include "assembler/assembler.h"
#include "ast/nodes/register.hpp"

using namespace avs8086::assembler;
using namespace avs8086::ast;

void Assembler::compile_mov(const QSharedPointer<Statement>& s)
{
    auto ss = qSharedPointerDynamicCast<MOV>(s);
    auto comma = qSharedPointerDynamicCast<Comma>(ss->m_expression);

    if (comma->m_expressions.length() != 2)
    {
        auto t = comma->m_token;
        addErrorInfo(
            t.row(), t.column(), t.literal().length(),
            "mov operand count error!"
        );
        return;
    }

    QString seg;
    QString format;

    auto e1 = comma->m_expressions.at(0);
    auto e2 = comma->m_expressions.at(1);
    if (e1->is(Node::NODE_COLON))
    {
        auto colon = qSharedPointerDynamicCast<Colon>(e1);
        seg = colon->m_left->value().toString();
        e1 = colon->m_right;
    }

    if (e1->is(Node::NODE_REGISTER))
    {
        if (e2->is(Node::NODE_REGISTER))
        {
            auto _e1 = qSharedPointerDynamicCast<Register>(e1);
            auto _e2 = qSharedPointerDynamicCast<Register>(e2);
            if (_e1->regType() == Register::SEGMENT
                && _e1->regType() == Register::SEGMENT)
            {
                auto t1 = _e1->m_token;
                auto t2 = _e2->m_token;
                addErrorInfo(
                    t1.row(), t1.column(), t2.column() + t2.literal().length(),
                    "mov can only one segment!"
                );
                return;
            }
            else if (_e1->regType() == Register::SEGMENT)
            {
                format = "10001110 modregr_m";
            }
            else if (_e2->regType() == Register::SEGMENT)
            {
                format = "10001100 modregr_m";
            }
            else if (_e1->regType() != _e2->regType())
            {
                auto t1 = _e1->m_token;
                auto t2 = _e2->m_token;
                addErrorInfo(
                    t1.row(), t1.column(), t2.column() + t2.literal().length(),
                    "reg must be same when not segment!"
                );
                return;
            }
            else
            {
                format = "100010dw modregr_m";
                format
                    .replace(
                        "w", _e1->regType() == Register::R_8BIT ? "0" : "1")
                    .replace("mod", "11").replace("d", "1")
                    .replace(
                        "reg", sm_reg_codes.value(_e1->value().toString()))
                    .replace(
                        "r_m", sm_reg_codes.value(_e2->value().toString()));
                int bin = format.toInt(nullptr, 2);
                // appendCode(QByteArray(QChar(bin & 0xff)));
                // appendCode(format);
                // qDebug() << format;
            }
        }
        else if (e2->is(Node::NODE_ADDRESS))
        {
            // TODO: 单独处理ac?
            format = "100010dw modregr_w";
        }
        else if (e2->is(Node::NODE_INTEGER) || e2->is(Node::NODE_STRING))
        {
            format = "1011wreg data";
        }
    }
    else if (e1->is(Node::NODE_ADDRESS))
    {
        if (e2->is(Node::NODE_REGISTER))
        {
            // TODO: 单独处理ac?
            format = "100010dw modregr_m";
        }
        else if (e2->is(Node::NODE_INTEGER) || e2->is(Node::NODE_STRING))
        {
            format = "1000111w mod000r_m data";
        }
    }
    else if (e1->is(Node::NODE_INTEGER) || e1->is(Node::NODE_STRING))
    {
        auto t = e1->m_token;
        addErrorInfo(
            t.row(), t.column(), t.literal().length(),
            "mov operand can not be imme on the first!"
        );
        return;
    }
    else
    {
        auto t = e1->m_token;
        addErrorInfo(
            t.row(), t.column(), t.literal().length(),
            "mov operand count error!"
        );
        return;
    }
}


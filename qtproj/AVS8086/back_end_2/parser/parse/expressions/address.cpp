#include "parser/parser.h"
#include "ast/nodes/address.hpp"
#include "ast/nodes/infix.hpp"

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_address()
{
    auto isImmediate = [](const QSharedPointer<Expression>& _e) {
        if (_e->valueIs(Expression::INTEGER)
            || _e->valueIs(Expression::STRING)) {
            int _n = _e->value().toInt();
            if (0 <= _n && _n <= 0xffff) {
                if (_n <= 0xff) {
                    return 1;
                }
                return 2;
            } else {
                return -1;
            }
        }
        return 0;
    }; // 0, -1

    auto isSingleReg = [](const QSharedPointer<Expression>& _e) {
        if (_e->is(Node::NODE_REGISTER)) { // single reg: SI, DI, BX
            // auto _r = qSharedPointerDynamicCast<Register>(_e);
            // auto _reg = _r->m_token.literal();
            auto _reg = _e->value().toString();
            if (_reg == "BX") {
                return 1;
            } else if (_reg == "BP") {
                return 2;
            } else if (_reg == "SI") {
                return 3;
            } else if (_reg == "DI") {
                return 4;
            } else {
                return -1;
            }
        }
        return 0;
    }; // 0, -1

    QString immeStr;
    QString the_str;

    auto isPlusInfix =
        [isImmediate, isSingleReg, &immeStr, &the_str]
        (const QSharedPointer<Expression>& _e) {
        if (_e->is(Node::NODE_INFIX)) { // 复杂的加法表达式
            auto _inf = qSharedPointerDynamicCast<Infix>(_e);
            if (_inf->op() != "+") {
                return -1;
            }

            auto _l = _inf->left();
            auto _r = _inf->right();
            QSharedPointer<Expression> _m;

            if (_l->is(Node::NODE_INFIX)) {
                _inf = qSharedPointerDynamicCast<Infix>(_l);
                _m = _inf->right(); // lr
                _l = _inf->left();  // ll
            } else if (_r->is(Node::NODE_INFIX)) {
                _inf = qSharedPointerDynamicCast<Infix>(_r);
                _m = _inf->left();  // rl
                _r = _inf->right(); // rr
            }

            if (_inf->op() != "+") {
                return -1;
            }

            auto _check =
                [isImmediate, isSingleReg, &immeStr, &the_str]
                (const QSharedPointer<Expression>& __e) {
                int _c;
                if ((_c = isSingleReg(__e))) {
                    the_str += __e->value().toString() + "+";
                    return _c; // -1, 1-4
                } else if ((_c = isImmediate(__e))) {
                    the_str += __e->value().toHex() + "+";
                    if (_c == -1) {
                        return -2;
                    } else if (_c == 1) {
                        immeStr = __e->value().toHex();
                        return 5;
                    } else {
                        immeStr = __e->value().toHex();
                        return 6;
                    }
                }
                return 0;
            };

            auto swch = [](int& n1, int& n2) {
                n1 ^= n2;
                n2 ^= n1;
                n1 ^= n2;
            };

            int _t[3] = {0, 0, 0};

            _t[0] = _check(_l);
            _t[1] = _check(_r);
            _t[2] = _m.isNull() ? 0x99 : _check(_m);

            for (int i = 0; i < 3; i++) {
                for (int j = i + 1; j < 3; j++) {
                    if (_t[i] > _t[j]) {
                        swch(_t[i], _t[j]);
                    }
                }
            }
            int code = 0;
            code |= _t[0];
            for (int i = 1; i < 3; i++) {
                code <<= 8;
                code |= _t[i];
            }
            return code;
        }
        return 0;
    }; // 0, -1, -2, -3, -4, -5


    int row = currToken().row();
    int col = currToken().column();
    int len = peekToken().column() - col + 1;
    nextToken();

    auto e = parse_expression(LOWEST);
    QSharedPointer<Address> addr(new Address);

    if (peekToken().is(Token::TOKEN_EOF))
    { // 此行'['未闭合
        addErrorInfo(row, col, 1, "'[' is not close on this line");
    }
    else if (!expectPeekToken(Token::TOKEN_RSQUARE))
    { // 括号没匹配上报错
        addExpectPeekTokenErrorInfo(Token::TOKEN_RSQUARE);
    }
    else
    { // 括号内 内容分析
        int check;
        if ((check = isImmediate(e)))
        { // 立即数
            if (check > 0)
            {
                addr->setValue(e->value());
            }
            else
            { // -1
                addErrorInfo(
                    row, col, len,
                    QString("integer should between 0H and ffffH: %1H")
                        .arg(e->value().toHex())
                );
            }
        }
        else if ((check = isSingleReg(e)))
        { // single reg: SI, DI, BX
            QString reg = e->value().toString();
            if (check && check != 2)
            { // SI, DI, BX
                addr->setValue(reg);
            }
            else
            { // -1, 2
                addErrorInfo(
                    row, col, len,
                    QString("single reg only be 'SI''DI''BX', illegal: %1")
                        .arg(reg)
                );
            }
        }
        else if ((check = isPlusInfix(e)))
        { // 复杂的加法表达式
            if (check > 0)
            {
                int c[3];
                for (int i = 2; i >= 0; i--)
                {
                    c[i] = check & 0xff;
                    check >>= 8;
                }
                bool flg = false;
                QString v = "";
                if (c[0] == 3 || c[0] == 4)
                {
                    v.append(c[0] == 3 ? "SI+" : "DI+");
                    if ((c[1] == 5 || c[1] == 6) && c[2] == 0x99)
                    {
                        flg = true;
                        v.append(immeStr);
                    }
                }
                else if (c[0] == 1 || c[0] == 2)
                {
                    v.append(c[0] == 1 ? "BX+" : "BP+");
                    int i = 1;
                    if (c[i] == 3 || c[i] == 4)
                    {
                        v.append(c[i] == 3 ? "SI" : "DI");
                        i++;
                    }
                    if (i == 2 || (c[i] == 5 || c[i] == 6))
                    {
                        flg = true;
                        if (c[i] == 5 || c[i] == 6)
                            v.append("+" + immeStr);
                    }
                }
                if (flg)
                {
                   addr->setValue(v);
                }
                else
                {
                    the_str.removeLast();
                    addErrorInfo(
                        row, col, len,
                        QString("can not match the '[%1]'").arg(the_str)
                    );
                }
            }
            else if (check == -1)
            {
                addErrorInfo(row, col, len, "must operator '+' when in '[]'");
            }
            else if (check == -2)
            {
                addErrorInfo(
                    row, col, len,
                    "statement must bez reg or imme when in '[]'"
                );
            }
            else if (check == -3)
            {
                addErrorInfo(
                    row, col, len,
                    QString("reg only be 'SI''DI''BX''BP' when in '[]',"
                            " illegal: %1")
                        .arg(e->value().toString())
                    );
            }
            else if (check == -4)
            {
                addErrorInfo(
                    row, col, len,
                    QString("integer should between 0H and ffffH: %1H")
                        .arg(e->value().toHex())
                );
            }
        }
        else
        { // error
            addErrorInfo(
                row, col, len,
                "unknow"
            );
        }
    }

    return addr;
}

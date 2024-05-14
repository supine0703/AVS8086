#include "parser/parser.h"
#include "ast/exprs/value.h"
#include <QRegularExpression>

using namespace avs8086::ast;
using namespace avs8086::token;
using namespace avs8086::parser;

/* ========================================================================== */

ExprPointer Parser::parse_value()
{
    auto type = currToken().type();
    if (type == Token::FLOAT)
    {
        addNotYetSupportErrorInfo();
        return ExprPointer(new Float(currToken()));
    }
    if (type == Token::INTEGER)
    {
        return ExprPointer(new Value(currToken(), currToken().toInt()));
    }
    if (type != Token::STRING)
    {
        Q_ASSERT_X(false, "Parser::parse_value", "value type error");
    }

    // 字符串转义
    static QHash<char, char> s_escapeTable = {
        { 'a', '\a' },  { 'b', '\b' },  { 'f', '\f' },  { 'n', '\n' },
        { 'r', '\r' },  { 't', '\t' },  { 'v', '\v' },  { '?', '\?' },
        { '\"', '\"' }, { '\'', '\'' }, { '\\', '\\' },
    };
    static const QRegularExpression s_all(
        R"(\\(?:[^ux0-7]|[0-7]+|[ux][0-9a-fA-F]+))"
    );
    static const QRegularExpression s_escape(
        R"(^\\([abfnrtv\'\"\\\?0]|0*[1-3]?[0-7]{0,2})"
        R"(|x0*[0-9a-fA-F]{1,2}|u0*[0-9a-fA-F]{1,4})$)"
    );

    QString str = (*currToken()).mid(1, currToken().length() - 2);
    auto e_it = s_all.globalMatch(str);
    QByteArray bytes;
    int last_i = 0;

    while (e_it.hasNext())
    {
        auto curr = e_it.next();

        bytes.append(str.mid(last_i, curr.capturedStart() - last_i).toUtf8());
        last_i = curr.capturedEnd();

        auto txt = curr.captured();
        auto match = s_escape.match(txt);
        if (!match.hasMatch())
        { // 错误的转义
            bytes.append(txt.toUtf8());
            int row = currToken().row();
            int col = currToken().column() + curr.capturedStart() + 2;
            int len = txt.length();
            QString info;
            if (txt.startsWith("\\x") || txt.startsWith("\\u"))
            {
                if (txt.length() == 1)
                    info = txt + " used with no following hex digits";
                else
                    info = "hex escape sequence out of range: " + txt;
            }
            else if (txt.length() == 1)
            {
                info = QString("unknown escape sequence '%1'").arg(txt);
            }
            else
            {
                info = "have unkonw escape sequence error";
            }
            addInfo(Info::ERROR, {row, col, len}, info);
            continue;
        }
        // 正确转义
        auto s = match.captured(1);
        if (s.startsWith('x'))
        {
            s.removeFirst();
            bytes.append(static_cast<char>(s.toUShort(nullptr, 16) & 0xff));
        }
        else if (s.startsWith('u'))
        {
            s.removeFirst();
            auto code = s.toUShort(nullptr, 16);
            bytes.append(static_cast<char>(code & 0xff));
            bytes.append(static_cast<char>((code >> 8) & 0xff));
        }
        else
        {
            bool ok;
            char octal = static_cast<char>(s.toUShort(&ok, 8) & 0xff);
            if (ok)
            { // 八进制数
                bytes.append(octal);
            }
            else
            { // 单个符号
                Q_ASSERT(s.length() == 1);
                bytes.append(s_escapeTable[s.at(0).unicode()]);
            }
        }
    }
    bytes.append(str.last(str.length() - last_i).toUtf8());

    return ExprPointer(new Value(currToken(), bytes));
}

/* ========================================================================== */

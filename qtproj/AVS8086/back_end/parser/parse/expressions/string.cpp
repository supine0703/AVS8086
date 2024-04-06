#include "parser/parser.h"
#include "ast/nodes/string.hpp"
#include <QRegularExpression>

using namespace avs8086::ast;
using namespace avs8086::parser;

QSharedPointer<Expression> Parser::parse_string()
{
    static const QRegularExpression all(
        R"(\\([^x0-7]|[0-7]+|x[0-9a-fA-F]+))"
    );
    static const QRegularExpression re(
        R"(^\\([abfnrtv\'\"\\\?0]|x[a-fA-F0-9]{1,2}|[0-7]{1,3})$)"
    );
    QString literal = currToken().literal();
    QString str = literal.removeFirst().removeLast();
    // 使用lambda函数替换所有匹配的转义字符
    auto it = all.globalMatch(str);
    while (it.hasNext())
    {
        auto curr = it.next();
        QString txt = curr.captured();
        auto match = re.match(txt);
        if (match.hasMatch())
        {
            auto s = match.captured(1);
            QString tos;
            if (s.startsWith('x'))
            {
                s.removeFirst();
                tos = QChar(s.toInt(nullptr, 16));
            }
            else
            {
                bool ok;
                int octal = s.toInt(&ok, 8);
                if (ok)
                {
                    tos = QChar(octal);
                }
                else
                {
                    switch (s.at(0).unicode())
                    {
                    case 'a':
                        tos = "\a";
                        break;
                    case 'b':
                        tos = "\b";
                        break;
                    case 'f':
                        tos = "\f";
                        break;
                    case 'n':
                        tos = "\n";
                        break;
                    case 'r':
                        tos = "\r";
                        break;
                    case 't':
                        tos = "\t";
                        break;
                    case 'v':
                        tos = "\v";
                        break;
                    case '\'':
                        tos = "\'";
                        break;
                    case '\"':
                        tos = "\"";
                        break;
                    case '\\':
                        tos = "\\";
                        break;
                    case '\?':
                        tos = "\?";
                        break;
                    default:
                        tos = 0;
                        break;
                    }
                }
            }
            str.replace(match.captured(), tos);
        }
        else
        {
            int row = currToken().row();
            int col = currToken().column() + literal.indexOf(txt) + 1;
            int len = txt.length();
            QString info;
            auto s = curr.captured(1);
            qDebug() << s;
            if (s.startsWith('x'))
            {
                if (s.length() == 1)
                    info = "\\x used with no following hex digits";
                else
                    info = "Hex escape sequence out of range: \\" + s;
            }
            else if (s.length() == 1)
            {
                info = QString("Unknown escape sequence '\\%1'").arg(s);
            }
            else
                info = "unkonw escape sequence error";

            addErrorInfo(row, col, len, info);
        }
    }
    return QSharedPointer<Expression>(new String(str));
}

#include "lexer/lexer.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

using namespace avs8086::token;
using namespace avs8086::lexer;

Lexer::Lexer()
    : Lexer("")
{ }

Lexer::Lexer(const QString& fileName)
{
    setFileName(fileName);
    scan();
}

Lexer::~Lexer()
{ }

/* ========================================================================== */

void Lexer::clear()
{
    m_file.clear();
    m_input.clear();
    m_labels.clear();
    m_errorInfos.clear();
    m_tokens.clear();
    m_tokenIt = 0;
    m_eofToken = Token(Token::TOKEN_EOF, "", 0, 0);
}

QString Lexer::fileName() const
{
    return m_file;
}

void Lexer::setFileName(const QString& fileName)
{
    clear();
    if (!fileName.isEmpty())
    {
        QFile inFile(m_file = fileName);
        if (!inFile.open(QIODevice::ReadOnly))
        {
            // TODO: translate
            addErrorInfo(0, 0, 0, "file open false: " + inFile.errorString());
            return;
        }
        else
        {
            QTextStream in(&inFile);
            // 结尾添加空白符:
            // 1. 确保最后一个token成功append; 2. eof column 不用额外 +1;
            m_input.append(in.readAll().append(' ').split('\n'));
            inFile.close();
        }
    }
}


bool Lexer::atEnd() const
{
    return m_tokenIt == m_tokens.length();
}

Token Lexer::next() const
{
    return atEnd() ? m_eofToken : m_tokens.at(m_tokenIt++);
}

Token Lexer::first() const
{
    m_tokenIt = 0;
    return next();
}

Token Lexer::end() const
{
    return m_eofToken;
}

QList<Token> Lexer::tokens() const
{
    return m_tokens;
}


bool Lexer::isError() const
{
    return !m_errorInfos.isEmpty();
}

QStringList Lexer::errorInfos() const
{
    return m_errorInfos;
}

/* ========================================================================== */

void Lexer::addErrorInfo(int row, int column, int len, const QString& info)
{
    m_errorInfos.append(
        QString("[%1:(%3,%4,%5)]>%2")
            .arg(m_file, info).arg(row).arg(column).arg(len)
    );
}


void Lexer::scan()
{
    if (isError())
        return;

    QString token;
    auto appendToken = [this, &token](int row, int column) {
        if (!token.isEmpty())
        {
            Token::Type type = Token::tokenType(token);
            if (type == Token::TOKEN_INTEGER && Token::lastTextToInt() == -2)
            {
                type = Token::TOKEN_ILLEGAL;
                addErrorInfo(
                    row, column - token.length(), token.length(),
                    "integer illegal: " + token
                );
            }
            m_tokens.append(
                Token(type, token, row, (column - token.length()))
            );
            token.clear();
        }
    };

    // 逐个扫描分离出 token
    for (int row = 0, end_r = m_input.length(); row < end_r; row++)
    {
        int _r = row + 1;
        const auto& line(m_input.at(row));
        for (int col = 0, end_c = line.length(); col < end_c; col++)
        {
            int _c = col + 1;
            const auto& ch(line.at(col));
            if (ch.isSpace())
            { // 空白字符 如果有token 则建立token
                appendToken(_r, _c);
                continue;
            }
            else if (ch == '\'' || ch == '"')
            { // 串类型
                appendToken(_r, _c);
                int len = textStrLen(line, col);
                if (len > 0)
                { // 串
                    token = line.mid(col, len + 2);
                    col += len + 1;
                    appendToken(_r, col + 2);
                }
                else
                { // 异常串
                    col = end_c;
                    if (len == -1)
                    { // 串界符没匹配上
                        // TODO: translate
                        addErrorInfo(
                            _r, _c, end_c - col,
                            "string is not closed on this line"
                        );
                    }
                    else if (len == 0)
                    { // 空串
                        // TODO: translate
                        addErrorInfo(_r, _c, 2, "string can not be empty");
                    }

                    m_tokens.append(Token(
                        Token::TOKEN_ILLEGAL,
                        line.last(col - _c),
                        _r, _c
                    ));
                    // qDebug() << "not closed:" << line.last(col - _c);
                }
            }
            else
            {
                if (_c < end_c)
                {
                    auto chType = Token::tokenType(ch, line.at(_c));
                    if (chType != Token::TOKEN_ILLEGAL)
                    { // 双目运算符
                        m_tokens.append(Token(
                            chType, QString(ch).append(line.at(_c)), _r, _c
                        ));
                        col++;
                        continue;
                    }
                }
                auto chType = Token::tokenType(ch);
                if (chType != Token::TOKEN_ILLEGAL)
                { // 是单目符号
                    appendToken(_r, _c);
                    if (chType == Token::TOKEN_ANNOTATION)
                    { // 注释
                        col = end_c;
                        token = line.last(col - _c + 1).trimmed();
                        appendToken(_r, col);
                        continue;
                        // qDebug() << "注释:"
                        //          << line.last(col - _c + 1).trimmed();
                    }
                    else if (chType == Token::TOKEN_COLON)
                    { // 标签 label
                        auto& last(m_tokens.last());
                        if (last.type() == Token::TOKEN_ILLEGAL
                            && last.row() == _r)
                        {
                            if (!QString("0123456789").contains(
                                    last.literal().at(0))
                                )
                            {
                                last.resetType(Token::TOKEN_LABEL);
                                m_labels.append(last.literal());
                            }
                            else
                            {
                                addErrorInfo(
                                    _r, last.column(), last.literal().length(),
                                    "label first can not be 0-9"
                                );
                            }
                        }
                    }
                    m_tokens.append(Token(chType, ch, _r, _c));
                }
                else
                { // 其他字符标识符
                    token.append(ch);
                }
            }
        }
    }

    // 二次遍历
    for (auto& t : m_tokens)
    {
        if (t.type() == Token::TOKEN_ILLEGAL)
        {
            const auto& tl = t.literal();
            if (m_labels.contains(tl))
            { // 标出标签
                t.resetType(Token::TOKEN_LABEL);
            }
            else if (tl.back() == 'H' || tl.back() == 'h')
            { // 检测不合法单词中是否可以辨别第一位不为0的数字
                auto new_tl = "0" + tl;
                if (Token::textToInt(new_tl) >= 0)
                {
                    t.resetType(Token::TOKEN_INTEGER);
                }
            }
            else
            {
                addErrorInfo(
                    t.row(), t.column(), tl.length(), "illegal token: " + tl
                );
            }
        }
    }

    m_eofToken = Token(
        Token::TOKEN_EOF, "",
        m_input.length(), m_input.at(m_input.length() - 1).length()
    );
}


int Lexer::textStrLen(const QString& line, int col)
{
    int len = -1;
    auto ch = line.at(col);
    if (ch == '\'' || ch == '"')
    {
        for (int i = col + 1, end = line.length(); i < end; i++)
        {
            if (line.at(i) == ch)
            {
                len = i - col - 1;
                break;
            }
        }
    }
    return len;
}


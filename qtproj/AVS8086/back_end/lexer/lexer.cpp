#include "lexer/lexer.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

using namespace avs8086::token;
using namespace avs8086::lexer;

Lexer::Lexer()
{ }

Lexer::Lexer(const QString& file, const QStringList& input)
{
    scan(file, input);
}

Lexer::~Lexer()
{ }

/* ========================================================================== */

void Lexer::clear()
{
    m_file.clear();
    m_errorInfos.clear();
    m_tokens.clear();
    m_tokenIt = 0;
    m_eofToken = Token(Token::TOKEN_EOF, "", 0, 0);
}

QString Lexer::fileName() const
{
    return m_file;
}

void Lexer::scan(const QString& file, const QStringList& input)
{
    clear();
    m_file = file;
    if (!input.isEmpty())
    {
        scan(input);
        return;
    }
    if (!m_file.isEmpty())
    {
        QFile inFile(m_file);
        if (!inFile.open(QIODevice::ReadOnly))
        {
            // TODO: translate
            addErrorInfo(0, 0, 0, "file open false: " + inFile.errorString());
            return;
        }
        else
        {
            QTextStream in(&inFile);
            QStringList f_input;
            while (!in.atEnd())
                f_input.append(in.readLine().append(' '));
            inFile.close();
            scan(f_input);
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


void Lexer::scan(const QStringList& input)
{
    if (isError() || input.isEmpty())
        return;

    QString tokenStr;
    QList<int> illegalIndex;
    QStringList labels;
    // TODO 没有对变量, 段名的扫描
    // QStringList segs;
    // QStringList vars;

    auto appendToken = [this, &tokenStr, &illegalIndex](int row, int column) {
        if (!tokenStr.isEmpty())
        {
            Token::Type type = Token::tokenType(tokenStr);
            if (type == Token::ILLEGAL || type == Token::ILLEGAL_INTEGER)
            {
                illegalIndex.append(m_tokens.length());
            }
            m_tokens.append(
                Token(type, tokenStr, row, (column - tokenStr.length()))
            );
            tokenStr.clear();
        }
    };

    // 逐个扫描分离出 token
    for (int row = 0, end_r = input.length(); row < end_r; row++)
    {
        int _r = row + 1;
        const auto& line(input.at(row));
        for (int col = 0, end_c = line.length(); col < end_c; col++)
        {
            int _c = col + 1;
            const auto& ch(line.at(col));
            /**
             * @bug: QPlainTextEdit的 '\n'会出现不可识别为空白字符的情况,
             *       可能是编码问题, 因此采用提前分好行的方式进行扫描
             */
            if (ch.isSpace())
            { // 空白字符 如果有token 则建立token
                appendToken(_r, _c);
                continue;
            }
            else if ((ch == '\'' || ch == '\"'))
            { // 串类型
                appendToken(_r, _c);
                int len = textStrLen(line, col);
                if (len > 0)
                { // 串
                    tokenStr = line.mid(col, len + 2);
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
                        Token::ILLEGAL,
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
                    if (chType != Token::ILLEGAL)
                    { // 双目运算符
                        appendToken(_r, _c);
                        m_tokens.append(Token(
                            chType, QString(ch).append(line.at(_c)), _r, _c
                        ));
                        col++;
                        continue;
                    }
                }
                auto chType = Token::tokenType(ch);
                if (chType != Token::ILLEGAL)
                { // 是单目符号
                    appendToken(_r, _c);
                    if (chType == Token::ANNOTATION)
                    { // 注释
                        col = end_c;
                        tokenStr = line.last(col - _c + 1).trimmed();
                        appendToken(_r, col);
                        continue;
                        // qDebug() << "注释:"
                        //          << line.last(col - _c + 1).trimmed();
                    }
                    else if (chType == Token::COLON)
                    { // 标签 label
                        auto& last(m_tokens.last());
                        if (last.is(Token::ILLEGAL)
                            && last.row() == _r)
                        {
                            auto last_l = last.literal();
                            last.resetType(Token::LABEL);
                            labels.append(last_l);
                            if (last_l.at(0).isDigit())
                            {
                                addErrorInfo(
                                    _r, last.column(), last.length(),
                                    "label first char can not be 0-9"
                                );
                            }
                        }
                        // 词法检测仅检测是否合法, 重复定义放到语法检测中处理
                    }
                    m_tokens.append(Token(chType, ch, _r, _c));
                }
                else
                { // 往后扫描, 构成 token
                    tokenStr.append(ch);
                }
            }
        }
    }

    // 二次遍历
    for (const int i : illegalIndex)
    {
        auto& t = m_tokens[i];
        auto tl = t.literal();
        if (t.is(Token::ILLEGAL_INTEGER))
        {
            t.resetType(Token::ILLEGAL);
            addErrorInfo(
                t.row(), t.column(), t.length(), "illegal integer: " + tl
            );
        }
        else if (labels.contains(tl))
        { // 标出标签
            t.resetType(Token::LABEL);
            if (tl.at(0).isDigit())
            {
                addErrorInfo(
                    t.row(), t.column(), t.length(),
                    "label first char can not be 0-9"
                );
            }
        }
        else if (tl.back().toUpper() == 'H' && Token::textToInt("0" + tl) >= 0)
        { // 检测不合法单词中是否可以辨别第一位不为0的数字
            t.resetType(Token::INTEGER);
        }
        else
        {
            addErrorInfo(
                t.row(), t.column(), tl.length(), "illegal token: " + tl
            );
        }
    }

    m_eofToken = Token(
        Token::TOKEN_EOF, "", input.length(), input.back().length()
    );
}


int Lexer::textStrLen(const QString& line, int col)
{
    int len = -1;
    const auto& ch = line.at(col);
    if (ch == '\'' || ch == '\"')
    {
        for (int i = col + 1, end = line.length(); i < end; i++)
        {
            if (line.at(i - 1) != '\\' && line.at(i) == ch)
            {
                len = i - col - 1;
                break;
            }
        }
    }
    return len; // -1: not string; 0: empty string;
}


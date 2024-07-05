#include "lexer.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

using namespace avs8086::token;
using namespace avs8086::lexer;

/* ========================================================================== */

void Lexer::clear()
{
    m_file.clear();
    m_infos.clear();
    m_tokens.clear();
    m_tokenIt = 0;
    m_eofToken = Token(Token::TOKEN_EOF, "", 0, 0);
}

void Lexer::scan(const QString& file, ScanInterface* scanner)
{
    clear();
    m_file = file;

    auto input = scanner->readFile();

    if (!input.isEmpty())
    {
        scan(input);
    }
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
            addErrorInfo({0, 0, 0}, "file open false: " + inFile.errorString());
            return;
        }
        QTextStream in(&inFile);
        QStringList f_input;
        /**
         * @bug: QPlainTextEdit的 '\n'会出现不可识别为空白字符的情况,
         *       可能是编码问题, 因此采用提前分好行的方式进行扫描
        */
        while (!in.atEnd())
            f_input.append(in.readLine());
        inFile.close();
        scan(f_input);
    }
}

Token Lexer::next() const
{
    if (atEnd())
        return m_eofToken;
    return m_tokens.at(m_tokenIt++);
}

Token Lexer::first() const
{
    m_tokenIt = 0;
    return next();
}

/* ========================================================================== */

QString Lexer::restore(const TokenList& tokens)
{
    Q_ASSERT(!tokens.isEmpty());
    auto& firstT = tokens.at(0);
    QString s = *firstT;
    int r = firstT.row();
    int c = firstT.endColumn();
    for (int i = 1, end = tokens.size(); i < end; i++)
    {
        auto& t = tokens.at(i);
        Q_ASSERT(!t.is(Token::TOKEN_EOF));
        if (r != t.row())
            s += QChar(QChar::LineFeed);
        else
            s += QString(t.column() - c, ' ');
        s += *t;
        r = t.row();
        c = t.endColumn();
    }
    return s;
}

/* ========================================================================== */

void Lexer::addErrorInfo(const Position& pos, const QString& info)
{
    // Info::Key key = { Info::ERROR, pos };
// #ifndef QT_NO_DEBUG
//     if (m_infos.contains(key))
//     {
//         auto it = m_infos.find(key);
//         auto [r, c, l] = it.key().second.value();
//         auto v = it.key().second.value();
//         qDebug()
//             << "(" << r << "," << c << "," << l << ")" << ":" << it.value();
//     }
// #endif
    m_infos.insert({Info::ERROR, pos, info});
}

void Lexer::addErrorInfo(const Token& token, const QString& info)
{
    addErrorInfo(
        token.pos(), QString("%1: %2").arg(info, *token)
    );
}

void Lexer::addErrorInfo(int row, int column, int length, const QString& info)
{
    addErrorInfo({row, column, length}, info);
}

/* ========================================================================== */

void Lexer::scan(const QStringList& input)
{
    if (isError() || input.isEmpty())
        return;

    bool addEOL = false;
    QString tokenTxt;
    QList<int> illIndex;
    QSet<QString> labels;
    // TODO: 定义常量和变量
    // QSet<QString> constants;
    // QSet<QString> variables;

    auto addIdError = [this](const Token& token) {
        addErrorInfo(
            token, "identifier first char cannot be 0-9"
        );
    };

    auto setId = [this, addIdError, &labels](int row) {
        auto i = m_tokens.size();
        if (i == 0)
            return;
        while (m_tokens.at(--i).is(Token::LINE_BREAK))
            ; // 跳过所有 '\'
        auto& last(m_tokens[i]);
        if (last.is(Token::ILLEGAL) && last.row() == row)
        {
            QString last_l = *last;
            last.resetType(Token::IDENTIFIER);
            if (last_l.at(0).isDigit())
            {
                addIdError(last);
            }
#if 0
            else if (labels.contains(last_l))
            {
                addErrorInfo(last, "redefined identifier");
            }
#endif
            else
            {
                labels.insert(last_l);
            }
        }
    };

    auto appendTxtToken =
        [this, &addEOL, &tokenTxt, &illIndex, setId](int row, int col) {
        if (!tokenTxt.isEmpty())
        {
            Token::Type type = Token::type(tokenTxt);
            if (type == Token::ILLEGAL || type == Token::ILLEGAL_INTEGER)
            {
                illIndex.append(m_tokens.size());
            }
            else if (type == Token::SEGMENT || type == Token::ALLOCATE)
            { // 定义 段, 值, 过程
                setId(row);
            }
            else if (type == Token::EQU)
            { // 常量

            }
            m_tokens.append(
                Token(type, tokenTxt, row, (col - tokenTxt.length()))
            );
            tokenTxt.clear();
            if (!addEOL && type != Token::ANNOTATION)
                addEOL = true;
        }
    };

    auto appendToken = [this, &addEOL](Token&& token) {
        m_tokens.append(token);
        if (!addEOL && token.is(Token::ANNOTATION))
            addEOL = true;
    };

    // 逐个扫描分离出 token
    const int end_r = input.size();
    for (int row = 0; row < end_r; row++)
    {
        int _r = row + 1;
        const auto& line(input.at(row));
        const int end_c = line.length();
        for (int col = 0; col < end_c; col++)
        {
            int _c = col + 1;
            const auto& ch(line.at(col));
            /**
             * @bug: QPlainTextEdit的 '\n'会出现不可识别为空白字符的情况,
             *       可能是编码问题, 因此采用提前分好行的方式进行扫描
            */
            if (ch.isSpace())
            { // 空白字符 如果有token 则建立token
                appendTxtToken(_r, _c);
                continue;
            }
            else if ((ch == '\'' || ch == '\"'))
            { // 串类型
                appendTxtToken(_r, _c);
                int len = textStrLen(line, col);
                if (len > 0)
                { // 串
                    tokenTxt = line.mid(col, len + 2);
                    col += len + 1;
                    appendTxtToken(_r, col + 2);
                }
                else
                { // 异常串
                    col = end_c; // break
                    appendToken({
                        Token::ILLEGAL,
                        line.last(col - _c),
                        _r, _c
                    });
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
                        addErrorInfo(_r, _c, 2, "string cannot be empty");
                    }
                }
            }
            else
            {
                if (_c < end_c)
                {
                    auto chType = Token::type(ch, line.at(_c));
                    if (chType != Token::ILLEGAL)
                    { // 双目运算符
                        appendTxtToken(_r, _c);
                        appendToken({
                            chType, QString(ch).append(line.at(_c)), _r, _c
                        });
                        col++;
                        continue;
                    }
                }
                auto chType = Token::type(ch);
                if (chType != Token::ILLEGAL)
                { // 是单目符号
                    appendTxtToken(_r, _c);
                    if (chType == Token::LINE_BREAK)
                    { // 换行继续
                        auto txt = line.last(end_c - _c + 1).trimmed();
                        if (txt.length() == 1)
                            col = end_c;
                        else
                        {
                            addErrorInfo(_r, _c, 1, "must be space after '\\'");
                        }
                    }
                    else if (chType == Token::ANNOTATION)
                    { // 注释
                        tokenTxt = line.last(end_c - _c + 1).trimmed();
                        appendTxtToken(_r, end_c);
                        break;
                    }
                    else if (chType == Token::COLON)
                    { // 标签 label
                        setId(_r);
                    }
                    else if (chType == Token::ASSIGN)
                    { // 变量

                    }
                    appendToken({chType, ch, _r, _c});
                }
                else
                { // 往后扫描, 构成 token
                    tokenTxt.append(ch);
                }
            }
        }
        appendTxtToken(_r, end_c + 1);
        // if (!m_tokens.isEmpty() && !m_tokens.back().is(Token::TOKEN_EOL))
        if (addEOL)
        {
            addEOL = false;
            m_tokens.append({Token::TOKEN_EOL, "\n", _r, end_c + 1});
        }
    }

    // 二次遍历
    for (const int i : illIndex)
    {
        auto& t = m_tokens[i];
        QString tl = *t;
        if (t.is(Token::ILLEGAL_INTEGER))
        {
            t.resetType(Token::ILLEGAL);
            addErrorInfo(t, "illegal integer");
        }
        else if (labels.contains(tl))
        { // 标出标识符
            t.resetType(Token::IDENTIFIER);
            if (tl.at(0).isDigit())
            {
                addIdError(t);
            }
        }
        else if (tl.back().toUpper() == 'H' && Token::textIsInteger(tl) == 1)
        { // 检测不合法单词中是否可以辨别第一位不为0的数字
            t.resetType(Token::INTEGER);
        }
        else
        {
            addErrorInfo(t, "illegal token");
        }
    }

    m_eofToken = Token(Token::TOKEN_EOF, "", end_r + 1, 1);
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

/* ========================================================================== */

#if 0
QString Lexer::fileName() const
{
    return m_file;
}

bool Lexer::atEnd() const
{
    return m_tokenIt == m_tokens.size();
}

Token Lexer::next() const
{
    if (atEnd())
        return m_eofToken;
    return m_tokens.at(m_tokenIt++);
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

TokenList Lexer::tokens() const
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
#endif

/* ========================================================================== */

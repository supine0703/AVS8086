#include "lexer/lexer.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>

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

QString Lexer::fileName()
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
            addErrorInfo(0, 0, "file open false: " + inFile.errorString());
            return;
        }
        else
        {
            QTextStream in(&inFile);
            while (!in.atEnd())
                m_input.append(in.readLine().append('\n'));
            inFile.close();
            m_tokens.append(Token(Token::TOKEN_PROGRAM, m_file, 0, 0));
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

void Lexer::addErrorInfo(int row, int column, const QString& info)
{
    m_errorInfos.append(
        QString("[%1:(%3,%4)]>%2").arg(m_file, info).arg(row).arg(column)
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
            int len = token.length();
            m_tokens.append(
                Token(Token::tokenType(token), token, row, (column - len))
            );
            token.clear();
        }
    };

    // 逐个扫描分离出 token
    for (int row = 0, end = m_input.length(); row < end; row++)
    {
        int _r = row + 1;
        const auto& line(m_input.at(row));
        for (int col = 0, end = line.length(); col < end; col++)
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
                {
                    col += len + 3;
                    token = line.mid(row - 1, len + 2);
                    appendToken(_r, col);
                }
                else
                {
                    col = end;
                    if (len == -1)
                    { // 串界符没匹配上
                        // TODO: translate
                        addErrorInfo(
                            _r, _c, "string is not closed on this line"
                        );
                    }
                    else if (len == 0)
                    { // 空串
                        // TODO: translate
                        addErrorInfo(_r, _c, "string can not be empty");
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
                Token::Type chType = Token::tokenType(ch.toLatin1());
                if (chType != Token::TOKEN_ILLEGAL)
                { // 是单目符号
                    appendToken(_r, _c);
                    if (chType == Token::TOKEN_ANNOTATION)
                    { // 注释
                        col = end;
                        token = line.last(col - _c + 1).trimmed();
                        appendToken(_r, col);
                        // qDebug() << "注释:"
                        //          << line.last(col - _c + 1).trimmed();
                    }
                    else if (chType == Token::TOKEN_COLON)
                    { // 标签 label
                        auto& last(m_tokens.last());
                        if (last.type() == Token::TOKEN_ILLEGAL
                            && last.row() == _r)
                        {
                            last.resetType(Token::TOKEN_LABEL);
                            m_labels.append(last.literal());
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

    // 遍历标出 label
    for (auto& t : m_tokens)
    {
        if (t.type() == Token::TOKEN_ILLEGAL
            && m_labels.contains(t.literal()))
        {
            t.resetType(Token::TOKEN_LABEL);
        }
    }

    auto the_end(m_input.at(m_input.length() - 1));
    if (!m_input.isEmpty())
        m_eofToken = Token(
            Token::TOKEN_EOF, "",
            m_input.length() + (the_end == "\n"), the_end.length()
        );
}

int Lexer::textToInt(const QString& numStr)
{
    static const QRegularExpression regex(
        "^(0X)?([0-9A-F]+?)([HDOB])?$",
        QRegularExpression::CaseInsensitiveOption
    );
    QRegularExpressionMatch match = regex.match(numStr.toUpper());

    if (match.hasMatch())
    {
        QString hex = match.captured(1);
        QString number = match.captured(2);
        QString suffix = match.captured(3);
        bool ok;
        int num, suf = 10;
        if (hex.isEmpty() || suffix.isEmpty())
        {
            if (!hex.isEmpty())
                suf = 16;
            else if (!suffix.isEmpty() && suffix != "D")
            {
                if (suffix == "H")
                    suf = 16;
                else if (suffix == "O")
                    suf = 8;
                else if (suffix == "B")
                    suf = 2;
            }
            num = number.toInt(&ok, suf);
            if (!ok)
                return num;
        }
        return -2;
    }
    return -1;
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


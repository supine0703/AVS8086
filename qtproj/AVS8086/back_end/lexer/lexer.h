#ifndef LEXER_H
#define LEXER_H

#include "token/token.h"

namespace avs8086::lexer {

class Lexer
{
public:
    Lexer();
    Lexer(const QString& file, const QStringList& input = {});
    ~Lexer() = default;

    void clear();

    QString fileName() const;

    /**
     * @param input: 如果为空, 则读取文件进行扫描, 否则扫描 input;
     */
    void scan(const QString& file, const QStringList& input = {});

    bool atEnd() const;
    token::Token next() const;
    token::Token first() const;
    token::Token end() const;
    QList<token::Token> tokens() const;

    bool isError() const;
    QStringList errorInfos() const;

    static QString restore(const QList<token::Token>& tokens);

private:
    void addErrorInfo(int row, int column, int len, const QString& info);

    void scan(const QStringList& input);

    static int textStrLen(const QString& line, int col); // -1: 不是串; 0: 空串;


private:
    QString m_file;
    QStringList m_errorInfos;

    QList<token::Token> m_tokens;
    token::Token m_eofToken;
    mutable int m_tokenIt;
};

} // namespace avs8086::lexer

#endif // LEXER_H

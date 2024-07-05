#ifndef LEXER_H
#define LEXER_H

#include "token/token.h"
#include "service/infos.h"

namespace avs8086::lexer {

class ScanInterface
{
public:
    virtual ~ScanInterface() = default;

    virtual QStringList readFile() = 0;
};

class Lexer
{
public:
    Lexer() : m_eofToken(token::Token::TOKEN_EOF, "", 0, 0) { }

    Lexer(const QString& file, const QStringList& input = {})
    { scan(file, input); }

    ~Lexer() = default;

    void clear();

    void scan(const QString& file, ScanInterface* scanner);


    /*
     * @param input: 如果为空, 则读取文件进行扫描, 否则扫描 input;
     * @param file: 需要解析的文件, 提示也是报错信息的目标
    */
    void scan(const QString& file, const QStringList& input = {});

    QString file() const { return m_file; }

    bool isError() const { return !m_infos.empty(); }

    InfoList infos() const { return m_infos; }

    token::TokenList tokens() const { return m_tokens; }

    bool atEnd() const { return m_tokenIt == m_tokens.size(); }

    token::Token end() const { return m_eofToken; }

    token::Token next() const;

    token::Token first() const;

    static QString restore(const token::TokenList& tokens);

private:
    void addErrorInfo(const Position& pos, const QString& info);
    void addErrorInfo(const token::Token& token, const QString& info);
    void addErrorInfo(int row, int column, int length, const QString& info);

    void scan(const QStringList& input);

    static int textStrLen(const QString& line, int col); // -1: 不是串; 0: 空串;


private:
    QString m_file;
    InfoList m_infos;

    mutable qsizetype m_tokenIt;
    token::TokenList m_tokens;
    token::Token m_eofToken;
};

} // namespace avs8086::lexer

#endif // LEXER_H

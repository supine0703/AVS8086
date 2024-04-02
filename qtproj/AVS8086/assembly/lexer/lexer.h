#ifndef LEXER_H
#define LEXER_H

#include "token/token.h"

class Lexer
{
public:
    Lexer();
    Lexer(const QString& fileName);
    ~Lexer();

    void clear();

    QString fileName();
    void setFileName(const QString& fileName);

    bool atEnd() const;
    Token next() const;
    Token first() const;
    Token end() const;
    QList<Token> tokens() const;

    bool isError() const;
    QStringList errorInfos() const;


private:
    void addErrorInfo(int row, int column, const QString& info);

    void scan();

    static int textToInt(const QString& numStr);
    static int textStrLen(const QString& line, int col);


private:
    QString m_file;
    QStringList m_input;
    QStringList m_labels;
    QStringList m_errorInfos;

    QList<Token> m_tokens;
    mutable int m_tokenIt;
    Token m_eofToken;
};

#endif // LEXER_H

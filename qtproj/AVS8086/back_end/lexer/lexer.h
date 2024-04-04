#ifndef LEXER_H
#define LEXER_H

#include "token/token.h"

namespace avs8086::lexer {

class Lexer
{
public:
    Lexer();
    Lexer(const QString& fileName);
    ~Lexer();

    void clear();

    QString fileName() const;
    void setFileName(const QString& fileName);

    bool atEnd() const;
    token::Token next() const;
    token::Token first() const;
    token::Token end() const;
    QList<token::Token> tokens() const;

    bool isError() const;
    QStringList errorInfos() const;


private:
    void addErrorInfo(int row, int column, int len, const QString& info);

    void scan();

    static int textStrLen(const QString& line, int col);


private:
    QString m_file;
    QStringList m_input;
    QStringList m_labels;
    QStringList m_errorInfos;

    QList<token::Token> m_tokens;
    mutable int m_tokenIt;
    token::Token m_eofToken;
};

} // namespace avs8086::lexer

#endif // LEXER_H

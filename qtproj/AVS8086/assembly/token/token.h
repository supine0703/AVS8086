#ifndef TOKEN_H
#define TOKEN_H

#include <QMap>

class Token
{
public:
    enum Type {
        TOKEN_ILLEGAL = 0,      // illegal
        TOKEN_EOF,              // eof
        TOKEN_PROGRAM,          // program
        TOKEN_REG8,             // 8bit register
        TOKEN_REG16,            // 16bit register
        TOKEN_LABEL,            // label
        TOKEN_STRING,           // string
        TOKEN_INTEGER,          // integer
        TOKEN_MNEMONIC,         // mnemonic
        TOKEN_INDICATE,         // indicate
        TOKEN_ANNOTATION,       // annotation ; ...
        TOKEN_LPAREN,           // (
        TOKEN_RPAREN,           // )
        TOKEN_LSQUARE,          // [
        TOKEN_RSQUARE,          // ]
        TOKEN_WELL,             // #
        TOKEN_COLON,            // :
        TOKEN_COMMA,            // ,
        TOKEN_PLUS,             // +
        TOKEN_MINUS,            // -
        TOKEN_ASTERISK,         // *
        TOKEN_SLASH,            // /
        TOKEN_EQUAL,            // =
        TOKEN_MAKE_,            // make_
        TOKEN_LOAD_,            // load_
    };

public:
    Token();
    Token(Type type, const QString& literal, int row, int column);
    Token(const Token& other);
    ~Token();

    Token& operator=(const Token& other);

    Type type() const;
    QString literal() const;
    int row() const;
    int column() const;

    void resetType(Type type);

    QString typeName() const;
    static QString tokenTypeName(Type type);
    static Type tokenType(char literal);
    static Type tokenType(const QString& literal);


private:
    Type m_type;
    QString m_literal;
    int m_row;
    int m_column;

    static const QMap<Type, QString> sm_typeNames;
    static const QStringList sm_makeNames;
    static const QStringList sm_loadNames;
    static const QStringList sm_reg8Names;
    static const QStringList sm_reg16Names;
    static const QStringList sm_mnemonicNames;
    static const QStringList sm_indicateNames;
};

#endif // TOKEN_H

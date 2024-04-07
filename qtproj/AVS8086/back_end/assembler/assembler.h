#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "token/token.h"
#include "ast/header.h"
HEADER_H

namespace avs8086::assembler {

class Assembler
{
public:
    enum CodeBit {
        BIT_D,
        BIT_W,
        BIT_REG,
        BIT_MOD,
        BIT_R_M,
        BIT_SW,
        BIT_V,
        BIT_Z,
    };

    // enum


public:
    Assembler();
    Assembler(const QSharedPointer<ast::Program>& root);
    ~Assembler();

    void setOutputPath(const QString& path);
    bool saveToFile() const;

    void compile();



private:
    void compile_single(const QSharedPointer<ast::Statement>& s);
    void compile_mov(const QSharedPointer<ast::Statement>& s);


    void addErrorInfo(int row, int col, int len, const QString& info);

    QByteArray& appendCode(const QByteArray& bytes);

    QByteArray binStrToByte(const QString& bin);



private:
    QString m_path;
    QString m_file;
    QByteArray m_codes;
    QSharedPointer<ast::Program> m_root;

    QStringList m_errorInfos;


    typedef
        void
        (Assembler::*compile_fn)(const QSharedPointer<ast::Statement>&);


    static const QMap<token::Token::Type, QByteArray> sm_single_codes_A;
    static const QMap<QByteArray, token::Token::Type> sm_single_codes_B;

    static const QMap<ast::Node::Type, compile_fn> sm_compile_fns;

    static const QMap<QString, QMap<CodeBit, QString>> sm_address_codes;

    static const QMap<QString, QString> sm_reg_codes;

};

} // namespace avs8086::assembler

#endif // ASSEMBLER_H

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

    enum OutType {
        OUT_BIN = 0,
        OUT_COM,
        OUT_EXE,
    };


    enum InitIndex {
        INIT_AX = 0,
        INIT_CX,
        INIT_DX,
        INIT_BX,
        INIT_SP,
        INIT_BP,
        INIT_SI,
        INIT_DI,
        INIT_ES,
        INIT_CS,
        INIT_SS,
        INIT_DS,
        INIT_IP,
        LOAD_SEGMENT,
        LOAD_OFFSET,
        INIT_SIZE,
    };

public:
    Assembler();
    Assembler(const QSharedPointer<ast::Program>& root);
    ~Assembler();

    void setOutputPath(const QString& path);
    bool saveToFile() const;

    OutType outType() const;

    QList<quint16> wellInitInfos() const;


    QStringList errorInfos() const;

    void compile();

    void copy(const QString& f1, const QString& f2);


private:

    void compile_single(const QSharedPointer<ast::Statement>& s);
    void compile_mov(const QSharedPointer<ast::Statement>& s);
    void compile_well(const QSharedPointer<ast::Statement>& s);


    void addErrorInfo(int row, int col, int len, const QString& info);

    QByteArray& appendCode(const QByteArray& bytes);

    QPair<QString, QString> compile_address(const QString& str);

    QList<quint16> m_initsList;


private:
    QString m_path;
    QString m_file;
    QByteArray m_codes;
    QSharedPointer<ast::Program> m_root;

    QStringList m_errorInfos;

    OutType m_outType;


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

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <QObject>

class Assembler : public QObject
{
    Q_OBJECT

private:
    enum UnitType {
        UnkonwUnit,
        Predefined,     // 预定义指令
        ULabel,         // 标签
        Instruction,   // 汇编/伪指令
        Annotation,     // 注释
    };

    enum TokenType {
        UnkonwToken,
        Symbol,     // 符号
        Mnemonic,   // 助记符
        Indicate,   // 伪指令
        Reg8,       // 8位寄存器
        Reg16,      // 16位寄存器
        Number,     // 数字
        TLabel,     // 标签
    };

    struct AsmUnit {
        int row;
        UnitType type;
        QString content;
        AsmUnit();
        AsmUnit(int row, UnitType type, const QString& content);
        bool operator==(const AsmUnit& other) const;
    };

    struct AsmToken {
        TokenType type;
        QString content;
        AsmToken();
        AsmToken(TokenType type, const QString& content);
    };

    struct AsmTokenList {
        int row;
        QList<AsmToken> tokens;
        AsmTokenList();
        AsmTokenList(int row, const QList<AsmToken>& tokens = {});
    };

public:
    explicit Assembler(QString asmFileName, QObject* parent = nullptr);

    void resetAsmFile(const QString& asmFileName);

    bool compile();

    bool readFile();

    void lexicalAnalysis();

    void syntaxAnalysis();

    void checkPredefined(const AsmUnit& unit);

    void checkLabel(const AsmUnit& unit);

    void checkInstruction(const AsmUnit& unit);

    int textToInt(const QString& numStr);

    void addError(int row, const QString& error);

    void addWarning(int row, const QString& warning);

private:
    QString asmFileName;
    QStringList asmText;
    QStringList successInfo;
    QStringList warningInfo;
    QStringList errorInfo;

    QList<AsmUnit> readyPredefined;
    QList<AsmUnit> readyULabel;
    QList<AsmTokenList> readyInstruction;


private:
    static const QStringList reg8_token;
    static const QStringList reg16_token;

    static const QStringList mnemonic_token;
    static const QStringList indicate_token;

signals:
};

#endif // ASSEMBLER_H

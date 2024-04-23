#include "assembler.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>


Assembler::Assembler(QString asmFileName, QObject* parent)
    : QObject{parent}
    , asmFileName(asmFileName)
{

}

void Assembler::resetAsmFile(const QString& asmFileName)
{
    this->asmFileName.clear();
    this->asmFileName = asmFileName;
}

bool Assembler::compile()
{
    if (!this->readFile())
        return false;

    lexicalAnalysis();
    syntaxAnalysis();

    qDebug() << "Error:";
    for (const auto& e : errorInfo) qDebug() << e;

    qDebug();
    qDebug() << "Warning:";
    for (const auto& w : warningInfo) qDebug() << w;
    return true;
}

bool Assembler::readFile()
{
    asmText.clear();
    errorInfo.clear();
    successInfo.clear();
    warningInfo.clear();

    QFile inFile(asmFileName);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // TODO: translate
        addError(-1, inFile.errorString());
        return false;
    }

    QTextStream in(&inFile);
    while (!in.atEnd())
    {
        QString line(in.readLine());
        asmText.append(line);
    }

    inFile.close();

    return true;
}

void Assembler::lexicalAnalysis()
{
    /**
     * @brief regex_1
     * 分成四个单元:
     *   1 - <#()#>  : 预定义指令  只能再行首 一行可多个
     *   2 - <():>   : 标签       行首或者特殊指令之后 一行可多个
     *   3 - <(.*?)> : 汇编/伪指令 一行至多只能有一条 后面只能跟注释或者无
     *   4 - <;()$>  : 注释       分号到行尾后都是注释
     * 1,2单元可能存在重复会被覆盖掉暂不提取
     * captured (提取出3,4单元):
     *   0: 符合条件的行语句
     *   1: 提取下一次匹配的串
     *   4: 汇编/伪指令
     *   5: 完整注释
     *   6: 注释内容
     */
    static const QRegularExpression regex_1(
        R"((^(\h*#[^;]*?#)*(\h*[^;]+?\h*:)*)\h*([^;]*?)\h*(;\h*(.*))?\h*$)"
    );
    /**
     * @brief regex_2
     * captured (提取所有1,2单元内容):
     *   0: 匹配到的单元(特殊指令或者标签)
     *   1: 单元1 - 预定义指令
     *   2: 取出单元1内容
     *   3: 单元2 - 标签
     *   4: 取出标签内容
     */
    static const QRegularExpression regex_2(
        "(#\\h*(.*?)\\h*#)|((\\S.*?)\\h*:)"
    );
    // 按行拆解内容 -> 单元

    QList<AsmUnit> unitList, upList, ulList, uiList, uaList;

    for (int i = 0, end = asmText.length(); i < end; i++)
    {
        auto match = regex_1.match(asmText.at(i)); // 一定会匹配成功 哪怕空白
        auto r = i + 1;
        // 过滤掉空行
        if (match.captured(0).isEmpty())
            continue;

        QString ui(match.captured(4).simplified()), ua(match.captured(6));

        auto it = regex_2.globalMatch(match.captured(1));
        while (it.hasNext())
        {
            match = it.next();
            if (match.hasMatch() && !match.captured(0).isEmpty())
            {
                if (!match.captured(2).isEmpty())
                {
                    AsmUnit u(r, UnitType::Predefined, match.captured(2));
                    unitList.append(u);
                    upList.append(u);
                }
                else if (!match.captured(4).isEmpty())
                {
                    AsmUnit u(r, UnitType::ULabel, match.captured(4));
                    unitList.append(u);
                    ulList.append(u);
                }
            }
        }
        if (!ui.isEmpty())
        {
            AsmUnit u(r, UnitType::Instruction, ui);
            unitList.append(u);
            uiList.append(u);
        }
        if (!ua.isEmpty())
        {
            AsmUnit u(r, UnitType::Annotation, ua);
            unitList.append(u);
            uaList.append(u);
        }
    }

    // 输出单元信息
    if (0)
    {
        int r = 0;
        for (const auto& u : unitList)
        {
            if (u.row != r)
            {
                r = u.row;
                qDebug();
                qDebug() <<
                    QString("%1: %2")
                        .arg(r, 2, 10, QChar('0')).arg(asmText.at(r - 1));
            }
            switch (u.type)
            {
            case Predefined:
                qDebug() << " 预定义指令:" << u.content;
                break;
            case ULabel:
                qDebug() << " 标签内容:" << u.content;
                break;
            case Instruction:
                qDebug() << " 汇编/伪指令:" << u.content;
                break;
            case Annotation:
                qDebug() << " 注释内容:" << u.content;
                break;
            default:
                break;
            }
        }
    }

    this->readyPredefined.clear();
    this->readyULabel.clear();

    for (const auto& u : upList)
        this->checkPredefined(u);
    for (const auto& u : ulList)
        this->checkLabel(u);
    for (const auto& u : uiList)
        this->checkInstruction(u);


    // for (const auto& u : unitList)
    // {
    //     switch (u.type)
    //     {
    //     case Predefined:
    //         this->checkPredefined(u);
    //         break;
    //     case Label:
    //         this->checkLabel(u);
    //         break;
    //     case Instructions:
    //         this->checkInstructions(u);
    //         break;
    //     case Annotation:
    //         // TODO format
    //         break;
    //     default:
    //         break;
    //     }
    // }
}

void Assembler::syntaxAnalysis()
{
    auto getRepeat = [](const QList<AsmUnit>& us) {
        QList<bool> usb(us.length(), true);
        QList<QList<int>> repeat;
        for (int i = 0, end = us.length() - 1; i < end; i++)
        {
            if (usb.at(i))
                for (int j = i + 1; j < us.length(); j++)
                    if (usb.at(j) &&
                        (us.at(i).content.split("=").at(0) ==
                         us.at(j).content.split("=").at(0)))
                    {
                        if (repeat.isEmpty() ||
                            repeat.at(repeat.length() - 1).at(0) != i)
                        {
                            repeat.append(QList<int>(1, i));
                        }
                        repeat[repeat.length() - 1].append(j);
                        usb[j] = false;
                    }
        }
        return repeat;
    };
    auto repP = getRepeat(readyPredefined);
    for (const auto& r : repP)
    {
        for (auto i : r)
        {
            // TODO: translate
            addWarning(
                readyPredefined.at(i).row,
                QString(
                    tr("The predefined is repeated: '%1', valid only the last")
                        .arg(readyPredefined.at(i).content)
                )
            );
        }
    }

    auto repUL = getRepeat(readyULabel);
    for (const auto& r : repUL)
    {
        for (auto i : r)
        {
            // TODO: translate
            addError(
                readyPredefined.at(i).row,
                QString(
                    tr("The Label redefined: '%1'")
                        .arg(readyPredefined.at(i).content)
                )
            );
        }
    }
}

void Assembler::checkPredefined(const AsmUnit& unit)
{
    static const QStringList make_token({
        "MAKE_BIN", "MAKE_COM", "MAKE_EXE",
    });
    static const QStringList load_token({
        "LOAD_SEGMENT", "LOAD_OFFSET"
    });

    AsmUnit u = unit;

    if (!make_token.contains(unit.content.toUpper()))
    {
        bool unkonwErr = false;
        auto ws = unit.content.split("=");
        if (ws.length() == 2)
        {
            auto k = ws.at(0).trimmed().toUpper();
            auto v = ws.at(1).trimmed();
            auto num = this->textToInt(v);
            bool numErr = false;
            if (num == -1)
                numErr = true;
            else if (reg16_token.contains(k) || load_token.contains(k))
            {
                if (!(0x0000 <= num && num <= 0xffff))
                    numErr = true;
            }
            else if (reg8_token.contains(k))
            {
                if (!(0x00 <= num && num <= 0xff))
                    numErr = true;
            }
            else
                unkonwErr = true;
            if (!numErr)
            {
                u.content = k + "=" + QString::number(num, 16);
            }
            else
            {
                // TODO: translate
                QString err(tr("the number is wrong in '%2': '%1'"));
                this->addError(unit.row, err.arg(v, unit.content));
            }
        }
        else
            unkonwErr = true;
        if (unkonwErr)
        {
            // TODO: translate
            QString err(tr("unkonw this predefined instruction: '%1'"));
            this->addError(unit.row, err.arg(unit.content));
        }
    }
    else
        u.content = unit.content.toUpper().replace("_", "_=");
    readyPredefined.append(u);
}

void Assembler::checkLabel(const AsmUnit& unit)
{
    static const QRegularExpression regex("^[A-Z_][0-9A-Z_@?]*$");

    AsmUnit u = unit;
    u.content = u.content.toUpper();

    auto match = regex.match(u.content);
    if (match.hasMatch())
    {
        readyULabel.append(u);
    }
    else
    {
        // TODO: translate
        QString err(tr("label do not conform to naming rules: '%1'"));
        this->addError(unit.row, err.arg(unit.content));
    }
}

void Assembler::checkInstruction(const AsmUnit& unit)
{
    QString symbol("-+*/=,?&$%()[]<>");
    static const QRegularExpression regex(
        "[-+*/=,?&$%()\\[\\]<>'\"]|[a-zA-Z0-9]+"
    );

    auto it = regex.globalMatch(unit.content);
    QStringList order;

    while (it.hasNext())
    {
        auto match = it.next();
        if (match.hasMatch())
            order.append(match.captured(0));
    }

    AsmTokenList asmt = AsmTokenList(unit.row);

    // readyInstruction.append(asmt);

    for (const auto& o : order)
    {
        TokenType type;
        if (symbol.contains(o))
        {
            type = TokenType::Symbol;
            asmt.tokens.append(AsmToken(TokenType::Symbol, o));
        }
        else if (reg8_token.contains(o))
        {
            type = TokenType::Reg8;
        }
        else if (reg16_token.contains(o))
        {
            type = TokenType::Reg16;
        }
        else if (mnemonic_token.contains(o))
        {
            type = TokenType::Mnemonic;
        }
        else if (indicate_token.contains(o))
        {
            type = TokenType::Indicate;
        }
        else if (textToInt(o) != -1)
        {
            type = TokenType::Number;
        }
        else
        { // 可能是定义的标签也可能违法
            bool err = true;
            for (const auto& l : readyULabel)
                if (l.content == o)
                {
                    err = false;
                    break;
                }
            if (!err)
            { // Label
                type = TokenType::Reg8;
            }
            else
            {
                type = TokenType::UnkonwToken;
                // TODO: translate
                QString err(tr("unkonw the word: '%1'"));
                this->addError(unit.row, err.arg(o));
            }
        }
        asmt.tokens.append(AsmToken(type, o));
    }

}

int Assembler::textToInt(const QString& numStr)
{
    static const QRegularExpression regex("^(-?[0-9a-fA-F]+?)([HDOB])?$");
    QRegularExpressionMatch match = regex.match(numStr.toUpper());

    if (match.hasMatch())
    {
        QString number = match.captured(1);
        QString suffix = match.captured(2);
        bool ok;
        int num, suf = 10;
        if (!suffix.isEmpty() && suffix != "D")
        {
            if (suffix == "H")
                suf = 16;
            else if (suffix == "O")
                suf = 8;
            else if (suffix == "B")
                suf = 2;
        }
        num = number.toInt(&ok, suf);
        if (ok)
            return num;
    }
    return -1;
}

void Assembler::addError(int row, const QString& error)
{
    errorInfo.append(QString("[%2:%1]>%3").arg(row).arg(asmFileName, error));
}

void Assembler::addWarning(int row, const QString& warning)
{
    warningInfo.append(
        QString("[%2:%1]>%3").arg(row).arg(asmFileName, warning)
    );
}


/* ========================================================================== */


Assembler::AsmUnit::AsmUnit() { }

Assembler::AsmUnit::AsmUnit(int row, UnitType type, const QString& content)
    : row(row), type(type), content(content) { }

bool Assembler::AsmUnit::operator==(const AsmUnit& other) const
{
    return (
        (this->row == other.row) &&
        (this->type == other.type) &&
        (this->content == other.content)
    );
}


Assembler::AsmToken::AsmToken() { }

Assembler::AsmToken::AsmToken(TokenType type, const QString& content)
    : type(type), content(content) { }


Assembler::AsmTokenList::AsmTokenList() { }

Assembler::AsmTokenList::AsmTokenList(int row, const QList<AsmToken>& tokens)
    : row(row), tokens(tokens) { }


/* ========================================================================== */

const QStringList Assembler::reg8_token({
    "AL", "AH", "BL", "BH", "CL", "CH", "DL", "DH",
});

const QStringList Assembler::reg16_token({
    "AX", "BX", "CX", "DX", "SP", "BP", "SI", "DI",
    "CS", "DS", "SS", "ES", "IP",
});

const QStringList Assembler::mnemonic_token({
    "AAA" , "AAD" , "AAM" , "AAS" , "ADC" , "ADD" , "AND" ,
    "CALL", "CBW" , "CLC" , "CLD" , "CLI" , "CMC" , "CMP" , "CMPS", "CWD" ,
    "DAA" , "DAS" , "DEC" , "DIV" ,
    "ESC" ,
    "HLT" ,
    "IDIV", "IMUL", "IN"  , "INC" , "INT" , "INTO", "IRET",
    "JA"  , "JNBE", "JAE" , "JNC" , "JNB" , "JB"  , "JC"  , "JNAE", "JBE" ,
    "JNA" , "JCXZ", "JE"  , "JZ"  , "JG"  , "JNLE", "JGE" , "JNL" , "JL"  ,
    "JNGE", "JLE" , "JNG" , "JMP" , "JNE" , "JNZ" , "JNO" , "JNP" , "JPO" ,
    "JNS" , "JO"  , "JP"  , "JPE" , "JS"  ,
    "LAHF", "LDS" , "LEA" , "LES" , "LOCK", "LODS", "LOOP","LOOPE","LOOPZ",
    "LOOPNE", "LOOPNZ",
    "MOV" , "MOVS","MOVSB", "MUL" , "NEG" , "NOP" , "NOT" , "OR"  , "OUT" ,
    "POP" , "POPF", "PUSH",
    "RCL" , "RCR" , "RET" , "REP" , "ROL" , "ROR" , "SAHF", "SAL" , "SBB" ,
    "SHL" , "SAR" , "SCAS", "STC" , "STD" , "STI" , "STOS", "SUB" ,
    "TEST",
    "XCHG", "XLAT", "XOR" , "WAIT",
});

const QStringList Assembler::indicate_token({
    "DD", "DW", "DD", "DQ", "DT", "EQU", "=", "LABEL", "SEGMENT", "ENDS",
    "ASSUME", "ORG", "NOTHING", "PROC", "NEAR", "FAR", "RET", "ENDP",
    "MACRO", "ENDM",
    // ...
});




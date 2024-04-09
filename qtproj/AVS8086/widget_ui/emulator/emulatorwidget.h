#ifndef EMULATORWIDGET_H
#define EMULATORWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include "FramelessWidget.h"
#include "vm/vm.h"
class InteractionWidget;

namespace Ui {
class emulatorwidget;
}

class emulatorwidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit emulatorwidget(QWidget *parent = nullptr);
    ~emulatorwidget();

    void setFile(const QString& file);

    void addSingle(const QByteArray& bytes);

    void singleRun();
    void allRun();
    void setBreaks(const QList<int> bs);

    void clearInc();


public slots:
    void runButton_clickedSlot();

private:
    void updateReg();
    void updateMem();
    void updateCode();

    InteractionWidget* inter;


    void setEdit();
    QString formatMem(const QString& hex);

    int csip();
    bool hlt;

    QList<int> m_bs = {2};
    int runCount;

    Ui::emulatorwidget *ui;

    // int currentHeadLine_l;
    // int currentHeadLine_r;//当前的第一行

    int m_row;
    int m_num;

    QString m_file;

    bool interOpen = false;

    avs8086::vm::VM vm;

    QStringList code1 = {
        "c70600107856",
        "c70602103412",
        QByteArray("\x26").toHex(),
        "c70600102143",
        "c70602106587",
        "bb0025",
        "be000b",
        "8b00",
        "01060010",
        "8b4002",
        "11060210",
        "f4",
        "00",
    };

    QStringList code2 = {
        "MOV [01000H], 05678H",
        "MOV [01002H], 01234H",
        "ES:",
        "MOV [02000H], 04321H",
        "MOV [02002H], 08765H",
        "MOV BX, 02500H",
        "MOV SI, 00B00H",
        "MOV AX, [BX+SI]",
        "ADD [01000H], AX",
        "MOV AX, [BX+SI+02H]",
        "ADC [01002H], AX",
        "HLT",
        "NOP",
    };


    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
};

#endif // EMULATORWIDGET_H

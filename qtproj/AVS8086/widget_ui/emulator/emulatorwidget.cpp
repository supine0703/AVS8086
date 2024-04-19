#include "emulatorwidget.h"
#include "ui_emulatorwidget.h"

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "assembler/assembler.h"
#include <QFileInfo>
#include <QTextBlock>
#include <QTextCursor>
#include <QMessageBox>
#include <QTimer>
#include "interactionwidget.h"

using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;
using namespace avs8086::assembler;
using namespace avs8086::vm;

emulatorwidget::emulatorwidget(QWidget *parent)
    : FramelessWidget("emu", parent, Func::Close)
    , ui(new Ui::emulatorwidget)
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();

    ui->setupUi(main_widget);
    FramelessWidget::setWidgetFixed(702, 562);
    main_widget->layout()->addWidget(ui->widget_5);
    this->setWindowFlags(this->windowFlags()/* | Qt::WindowStaysOnTopHint*/);

    setEdit();
    this->hide();

    inter = new InteractionWidget(this);
    inter->setVisible(false);
}

emulatorwidget::~emulatorwidget()
{
    delete ui;
    inter->deleteLater();
}

void emulatorwidget::setFile(const QString& file)
{
    qDebug() << "file:" << file;
    m_file = file;
    QFileInfo info(file);


    Lexer l(file);
    Parser p(&l);

    auto root = p.newAST();

    Assembler a(root);
    a.compile();


    a.copy(CONFIG_PATH"mycode.bin",
           info.path() + "/main.bin");

    vm.m_regs = a.wellInitInfos();

    vm.readFromFile(info.path() + "/main.bin");
    // qDebug() << info.path() + "/main.bin";

    updateReg();
    updateMem();

    inter->breaksEdit->setPlainText(ui->asmCode->toPlainText());

    runCount = 0;
}

void emulatorwidget::addSingle(const QByteArray& bytes)
{
    int pc = csip();
    for (const auto& b : bytes)
    {
        vm.m_memory[pc++] = b;
    }
    updateMem();
    updateCode();
}

void emulatorwidget::singleRun()
{
    QByteArray bytes = vm.m_memory.mid(csip(), 6);
    int index, len;
    for (int i = 6; i >= 0; i--)
    {
        QString b = bytes.mid(0, i).toHex();
        if (i == 0)
        {
            // qDebug() << vm.m_memory.mid(csip(), 6).toHex();
            exit(-1);
        }
        if (code1.contains(b))
        {
            // qDebug() << code2.at(code1.indexOf(b));
            index = code1.indexOf(b);
            len = b.length() / 2;
            vm.REG(VM::IP) += len;
            break;
        }
    }

    switch(index)
    {
    case 0:
        vm.m_memory[vm.REG(VM::DS) * 16 + 0x1000] = 0x78;
        vm.m_memory[vm.REG(VM::DS) * 16 + 0x1001] = 0x56;
        break;
    case 1:
        vm.m_memory[vm.REG(VM::DS) * 16 + 0x1002] = 0x34;
        vm.m_memory[vm.REG(VM::DS) * 16 + 0x1003] = 0x12;
        break;
    // case 2:
    case 3:
        vm.m_memory[vm.REG(VM::ES) * 16 + 0x1000] = 0x12;
        vm.m_memory[vm.REG(VM::ES) * 16 + 0x1001] = 0x34;
        break;
    case 4:
        vm.m_memory[vm.REG(VM::ES) * 16 + 0x1002] = 0x56;
        vm.m_memory[vm.REG(VM::ES) * 16 + 0x1003] = 0x78;
        break;
    case 5:
        vm.REG(VM::BX) = 0x1500;
        break;
    case 6:
        vm.REG(VM::SI) = 0x0b00;
        break;
    case 7:
        // vm.REG(VM::AX) = vm.m_memory[vm.REG(VM::BX) + vm.REG(VM::SI) + vm.REG(VM::DS) * 16];
        vm.REG(VM::AX) = 0x4321;
        break;
    case 8:
        vm.m_memory[vm.REG(VM::DS) * 16 + 0x1000] = vm.REG(VM::AX);
        vm.m_memory[vm.REG(VM::DS) * 16 + 0x1001] = vm.REG(VM::AX) >> 8;
        break;
    case 9:
        // vm.REG(VM::AX) = vm.m_memory[vm.REG(VM::BX) + vm.REG(VM::SI) + vm.REG(VM::DS) * 16 + 2];
        vm.REG(VM::AX) = 0x8765;
        break;
    case 10:
        // vm.m_memory[vm.REG(VM::DS) * 16 + 0x1000] = vm.REG(VM::AX) + 1;
        vm.m_memory[0x6000] = 0x99;
        vm.m_memory[0x6001] = 0x99;
        vm.m_memory[0x6002] = 0x99;
        vm.m_memory[0x6003] = 0x99;
        break;
    case 11:
        QMessageBox::information(this, "提示", "虚拟机已暂停");
        vm.REG(VM::IP) -= len;
        hlt = true;
        break;
    defualt:
        exit(-2);
    }
    updateReg();
    updateMem();
    updateCode();
    runCount++;
}

void emulatorwidget::allRun()
{
    if (hlt == false)
    {
        singleRun();
        if (!m_bs.contains(runCount + 1))
        {
            QTimer* t = new QTimer(this);
            t->start(10);
            connect(t, &QTimer::timeout, this, [this, t] () {
                t->deleteLater();
                allRun();
            });
        }
    }
}

void emulatorwidget::setBreaks(const QList<int> bs)
{
    m_bs = bs;
}

void emulatorwidget::clearInc()
{
    int pc = csip();
    for (int i = 0; i < 16; i++)
    {
        vm.m_memory[pc++] = 0;
    }
    updateMem();
    updateCode();
}

void emulatorwidget::runButton_clickedSlot()
{
    this->show();
    updateReg();
    updateMem();
}

void emulatorwidget::updateReg()
{
    // qDebug() << vm.m_regs;
    // qDebug() << QString("%1").arg(((quint8)(vm.REG(VM::AX) >> 8) & 0xff), 2, 16, QChar('0'));
    ui->AH->setText(QString("%1").arg(((quint8)(vm.REG(VM::AX) >> 8) & 0xff), 2, 16, QChar('0')));
    ui->AL->setText(QString("%1").arg(((quint8)(vm.REG(VM::AX) & 0xff)), 2, 16, QChar('0')));
    ui->BH->setText(QString("%1").arg(((quint8)(vm.REG(VM::BX) >> 8) & 0xff), 2, 16, QChar('0')));
    ui->BL->setText(QString("%1").arg(((quint8)(vm.REG(VM::BX) & 0xff)), 2, 16, QChar('0')));
    ui->CH->setText(QString("%1").arg(((quint8)(vm.REG(VM::CX) >> 8) & 0xff), 2, 16, QChar('0')));
    ui->CL->setText(QString("%1").arg(((quint8)(vm.REG(VM::CX) & 0xff)), 2, 16, QChar('0')));
    ui->DH->setText(QString("%1").arg(((quint8)(vm.REG(VM::DX) >> 8) & 0xff), 2, 16, QChar('0')));
    ui->DL->setText(QString("%1").arg(((quint8)(vm.REG(VM::DX) & 0xff)), 2, 16, QChar('0')));

    ui->SP->setText(QString("%1").arg((quint16)vm.REG(VM::SP), 4, 16, QChar('0')));
    ui->BP->setText(QString("%1").arg((quint16)vm.REG(VM::BP), 4, 16, QChar('0')));
    ui->SI->setText(QString("%1").arg((quint16)vm.REG(VM::SI), 4, 16, QChar('0')));
    ui->DI->setText(QString("%1").arg((quint16)vm.REG(VM::DI), 4, 16, QChar('0')));
    ui->ES->setText(QString("%1").arg((quint16)vm.REG(VM::ES), 4, 16, QChar('0')));
    ui->CS->setText(QString("%1").arg((quint16)vm.REG(VM::CS), 4, 16, QChar('0')));
    ui->SS->setText(QString("%1").arg((quint16)vm.REG(VM::SS), 4, 16, QChar('0')));
    ui->DS->setText(QString("%1").arg((quint16)vm.REG(VM::DS), 4, 16, QChar('0')));
    ui->IP->setText(QString("%1").arg((quint16)vm.REG(VM::IP), 4, 16, QChar('0')));

    ui->seg1->setText(QString("%1").arg((quint16)vm.REG(VM::CS), 4, 16, QChar('0')));
    ui->of1->setText(QString("%1").arg((quint16)vm.REG(VM::IP), 4, 16, QChar('0')));
}

void emulatorwidget::updateMem()
{
    QString mem;

    int pc = ui->seg1->text().toInt(nullptr, 16) * 16 + ui->of1->text().toInt(nullptr, 16);

    bool big = pc >= 0x02000;
    bool small = pc < 0xfdfff;

    int start =  big ? pc - 0x02000 : pc;
    int end = small ? pc + 0x2000 : 0xfffff;

    m_row = big ? 0x2000 : pc;

    for (int i = start; i < end; i++)
    {
        QString b = QString("%1").arg((quint8)vm.m_memory.at(i), 2, 16, QChar('0'));
        mem.append(QString("%1: %2\n").arg(i, 5, 16, QChar('0')).arg(b));
    }

    ui->member->setPlainText(mem);

    QTextBlock textBlock = ui->member->document()->findBlockByLineNumber(0x2000);
    QTextCursor cursor = QTextCursor(textBlock);
    QTextBlockFormat blockFormat = cursor.blockFormat();
    if (pc == csip())
    {
        blockFormat.setBackground(QBrush(Qt::yellow));
        cursor.setBlockFormat(blockFormat);
        for(int i = 0; i < m_num; ++i) {
            cursor.setBlockFormat(blockFormat);
            cursor.movePosition(QTextCursor::NextBlock);
        }
    }
    else
    {   blockFormat.setBackground(QBrush(Qt::darkYellow));
        cursor.setBlockFormat(blockFormat);
    }
    for (int i = m_num; i < 0x15; i++)
        cursor.movePosition(QTextCursor::NextBlock);
    ui->member->setTextCursor(cursor);
}

void emulatorwidget::updateCode()
{
    QByteArray bytes = vm.m_memory.mid(csip(), 0x40);
    QString c;
    m_num = 0;
    // qDebug() << bytes;
    while (!bytes.isEmpty())
    {
        int i = 6;
        for (; i > 0; i--)
        {
            QString b = bytes.mid(0, i).toHex();
            if (code1.contains(b))
            {
                // qDebug() << code2.at(code1.indexOf(b));
                c.append(code2.at(code1.indexOf(b)));
                c.append("\n");
                bytes = bytes.mid(i, bytes.length() - i);
                if (m_num == 0)
                    m_num = b.length() / 2;
                break;
            }
        }
        if (i == 0)
        {
            // qDebug() << "f:" << "NOP";
            bytes = bytes.mid(1, bytes.length() - i);
        }
    }
    ui->asmCode->setPlainText(c);

    auto setColor = [](auto& edit, int row) {
        QTextCursor cursor(edit->document()->findBlockByNumber(row));
        QTextBlockFormat blockFormat = cursor.blockFormat();
        blockFormat.setBackground(QBrush(Qt::yellow));
        cursor.setBlockFormat(blockFormat);
    };

    setColor(ui->asmCode, 0);

    setColor(inter->breaksEdit, 0);

    // // 假设你有一个QPlainTextEdit对象名为plainTextEdit
    // QTextCursor cursor(ui->member->document());
    // QTextBlockFormat blockFormat;
    // blockFormat.setBackground(QBrush(Qt::yellow)); // 设置背景色为黄色

    // // 从第r行到第n行设置背景色
    // int r = 5; // 起始行号，从0开始计数
    // int n = 10; // 结束行号
    // cursor.setPosition(ui->member->document()->findBlockByNumber(r).position());
    // for(int i = r; i <= n; ++i) {
    //     cursor.setBlockFormat(blockFormat);
    //     cursor.movePosition(QTextCursor::NextBlock);
    // }
}

void emulatorwidget::setEdit()
{
    ui->member->setLineWrapMode(QPlainTextEdit::NoWrap);//设置不自动换行
    ui->asmCode->setLineWrapMode(QPlainTextEdit::NoWrap);
    connect(ui->seg1, &QLineEdit::textChanged, this, [this]() {
        updateMem();
        ui->seg2->setText(ui->seg1->text());
        updateCode();
    });

    connect(ui->of1, &QLineEdit::textChanged, this, [this]() {
        updateMem();
        ui->of2->setText(ui->of1->text());
        updateCode();
    });

    connect(ui->reloadButton, &QToolButton::clicked, this, [this]() {
        setFile(m_file);
        updateCode();
    });

    connect(ui->singleButton, &QToolButton::clicked, this, [this]() {
        singleRun();
    });

    connect(ui->runButton, &QToolButton::clicked, this, [this]() {
        hlt = false;
        allRun();
    });

    connect(ui->debugbutton, &QToolButton::clicked, this, [this]() {
        interOpen = !interOpen;
        inter->setVisible(interOpen);
    });

}

int emulatorwidget::csip()
{
    return vm.REG(VM::CS) * 16 + vm.REG(VM::IP);
}

// QString emulatorwidget::formatMem(const QString& hex)
// {
//     return QString("%1:").arg(hex);
// }

void emulatorwidget::mouseMoveEvent(QMouseEvent* event)
{
    FramelessWidget::mouseMoveEvent(event);
}

void emulatorwidget::mousePressEvent(QMouseEvent* event)
{
    FramelessWidget::mousePressEvent(event);
}


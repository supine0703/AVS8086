#include <QTextBlock>
#include <QRegularExpression>
#include "debugwidget.h"
#include "ui_debugwidget.h"


Debugwidget::Debugwidget(QWidget *parent)
    : QPlainTextEdit(parent)
    , errStringList(QStringList())
    , warnStringList(QStringList())
    , errlineList(QStringList())
    , warnlineList(QStringList())
    , ui(new Ui::debugwidget)
{
    ui->setupUi(this);

    stringToLine();

    this->setReadOnly(true);//设置只读

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->setLineWrapMode(QPlainTextEdit::NoWrap);//设置不会自动换行

    this->setFrameShape(QFrame::NoFrame);

    connect(this->document(), &QTextDocument::contentsChanged, this, [this](){
        stringToLine();
    });
}
Debugwidget::~Debugwidget()
{
    delete ui;
}

QStringList* Debugwidget::getErrlineList()
{
    return &errlineList;
}

QStringList* Debugwidget::getWarnlineList()
{
    return &warnlineList;
}

void Debugwidget::add_()
{
    this->clear();
    for (const auto& s : errStringList)
    {
        this->appendPlainText(s);
    }

    for (const auto& s : warnStringList)
    {
        this->appendPlainText(s);
    }
}


void Debugwidget::matchNumber(int& errLine, int& errWord, int& errWordLengh, QString tmp)
{
    // 0: all; 1: file; 2,3,4: row, column, len; 5:info
    static const QRegularExpression re(R"(^\[(.*?)\((\d+),(\d+),(\d+)\)\]>(.*?)$)");
    QRegularExpressionMatch match = re.match(tmp);

    if (match.hasMatch())
    {
        errLine = match.captured(2).toInt();
        errWord = match.captured(3).toInt();
        errWordLengh = match.captured(4).toInt();

        qDebug() << "Number 1: " << errLine;
        qDebug() << "Number 2: " << errWord;
        qDebug() << "Number 3: " << errWordLengh;
    }
    else
    {
        qDebug()<<tmp;
        qDebug()<<"读取失败！\n"<< __FILE__ << __LINE__;;
        return;
    }

    if (errLine == 0 || errWord == 0 || errWordLengh == 0)
    {
        qDebug()<<tmp;
        qDebug()<<"读取错误！\n"<< __FILE__ << __LINE__;;
        return;
    }
}

void Debugwidget::stringToLine()
{
    if (!errStringList.isEmpty())
    {
        int errLine = 0, errWord = 0, errWordLengh = 0;
        for (int i = 0; i < errStringList.length(); i++)
        {
            matchNumber(errLine, errWord, errWordLengh, errStringList.at(i));

            if (errLine == 0 || errWord == 0 || errWordLengh == 0)
            {
                qDebug()<<"读取错误！\n"<< __FILE__ << __LINE__;;
                return;
            }

            errlineList.append(QString::number(errLine) + " "
                               + QString::number(errWord) + " " + QString::number(errWordLengh));//相当于拷贝副本
        }
    }

    //计算出warn行
    if (!warnStringList.isEmpty())
    {
        int warnLine = 0, warnWord = 0, warnWordLengh = 0;
        for (int i = 0; i < warnStringList.length(); i++)
        {
            matchNumber(warnLine, warnWord, warnWordLengh, warnStringList.at(i));
            if (warnLine == 0 || warnWord == 0 || warnWordLengh == 0)
            {
                qDebug()<<"读取错误！\n"<< __FILE__ << __LINE__;;
                return;
            }

            warnlineList.append(QString::number(warnLine) + " "
                                + QString::number(warnWord) + " " + QString::number(warnWordLengh));//相当于拷贝副本
        }
    }
}

void Debugwidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    int errLine = 0, errWord = 0, errWordLengh = 0;

    QTextCursor cursor = this->textCursor();
    QTextBlock currentBlock = cursor.block();

    QString tmp = currentBlock.text();//获取

    matchNumber(errLine, errWord, errWordLengh, tmp);//调用方法，引用调用

    emit errLineNumber(errLine, errWord, errWordLengh);
}

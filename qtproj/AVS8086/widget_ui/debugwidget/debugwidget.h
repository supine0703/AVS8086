#ifndef DEBUGWIDGET_H
#define DEBUGWIDGET_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QStringList>
#include <QList>

namespace Ui {
class debugwidget;
}

class Debugwidget : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit Debugwidget(QWidget *parent = nullptr);
    ~Debugwidget();
    QStringList* getErrlineList();
    QStringList* getWarnlineList();

    QString filefata;


    QStringList errStringList;//存放错误以及警告信息的List
    QStringList warnStringList;

    void add_();


signals:
    void errLineNumber(int errLine, int errWord, int errWordLengh);//向mainwidget返回错误行号

private:
    Ui::debugwidget *ui;

    // QStringList errStringList;//存放错误以及警告信息的List
    // QStringList warnStringList;

    QStringList errlineList;
    QStringList warnlineList;

    void matchNumber(int& errLine, int& errWord, int& errWordLengh, QString tmp);

    void stringToLine();

    // QWidget interface
protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
};

#endif // DEBUGWIDGET_H

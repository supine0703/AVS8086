#ifndef NEWWIDGET_H
#define NEWWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QTableWidget>
#include "mylineedit.h"
#include "FramelessWidget.h"
//#include "filetreewidget.h"

namespace Ui {
class newwidget;
}

class QRegularExpression;

class Newwidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit Newwidget(QWidget *parent = nullptr);
    ~Newwidget();

    MyLineEdit* getEditPath();
    MyLineEdit* getEditPro();
    void editClear();

    QTableWidget* getTableWidget();

signals:
    // void buildFile(QString name, QString path, QString suffix);

    void PathEditContent(QString path);//传递选择的文件夹
    void createAsmChecked();//

private:
    void pressedStyle(QToolButton* button);
    void resetStyleButton(QWidget* widget);
    void setButton();
//    QString

    QStringList* getLocalDrives();

    Ui::newwidget *ui;
    QStringList* localDrives;
    QRegularExpression pathReg;
    QRegularExpression proReg;

    QString editWidgetStyle;
    QString errEditStyle;

    MyLineEdit* lineEditPath;
    MyLineEdit* lineEditPro;

    QString suffix;
    bool pathIsErr;
    bool proIsErr;

    int selectRow;
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent* event) override;

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent* event) override;
};

#endif // NEWWIDGET_H

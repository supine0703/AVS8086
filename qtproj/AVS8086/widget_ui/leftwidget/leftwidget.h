#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H

#include <QWidget>

namespace Ui {
class leftwidget;
}

class QToolButton;

class Leftwidget : public QWidget
{
    Q_OBJECT

public:
    explicit Leftwidget(QWidget *parent = nullptr);
    ~Leftwidget();

    void initButton();
    void setButton();
    void setMenu();
    void initStyle();
    void initList();
    void setImages();

    QToolButton* getFuncButton();

signals:
    void funcButton_clicked();
    void proButton_clicked();
    void runButton_clicked();

private:
    Ui::leftwidget *ui;

    QString style1;
    QString style2;

    QList<QToolButton*> ListButtons;

};

#endif // LEFTWIDGET_H

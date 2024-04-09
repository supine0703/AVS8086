#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QMouseEvent>
#include <QMenuBar>
#include <QWidget>
#include "MyMenu.h"

namespace Ui {
class titlebar;
}

class Titlebar : public QWidget
{
    Q_OBJECT

public:
    Titlebar(QWidget *parent = nullptr);
    ~Titlebar();
    void InitDesigner();

protected:
    virtual void changeEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

protected:
    QPoint mousePos;
    QPoint wndPos;
    bool mousePressed;
    bool menuButtonPressed = false;
    int last_width;

    //UI界面控件的信号槽函数，可自定义重新设计
private slots:
    void on_MiniButton_clicked();

    void on_MaxButton_clicked();

    void on_CloseButton_clicked();

    void on_RecoverButton_clicked();

    void on_MenuButton_clicked();


private:
    Ui::titlebar *ui;
    QWidget *parent_MainWindow;
    QString buttonStyle;
    QImage *img = new QImage;

    QMenuBar *menuBar;

private:
    void setMenu();
    void addMenuTobar(MyMenu* menuBar);
    MyMenu* createMenu(const QStringList &actions, QWidget* parent);
    QAction* createAction(const QString &text, QWidget* parent);
    void menuBarMove();
};

#endif // TITLEBAR_H

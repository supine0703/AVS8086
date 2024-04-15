#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QMenuBar>
#include <QLabel>
#include <QDialog>
#include <QTimer>

enum Location {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER
};

enum Func {
    Max,
    Min,
    Close,
    All
};

namespace Ui {
class FramelessWidget;
}

class QToolButton;

class FramelessWidget : public QDialog
{
    Q_OBJECT

public:
    explicit FramelessWidget(const QString& name, QWidget *parent = nullptr, Func func = All);
    ~FramelessWidget();
    void InitDesigner();

protected:
    QWidget* getMain_Widget();
    QLabel* getmenu_label();
    QWidget* getMenuwidget();
    void setWidgetFixed(int w, int h);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    virtual void changeEvent(QEvent *event) override;

signals:
    void clickOnceEvent();
    void clickTwiceEvent();
    void menuButtonClicked();
    void newFileButtonClicked(QAction* action);
    void openFileButtonClicked();

    //UI界面控件的信号槽函数，可自定义重新设计
private slots:
    void on_MiniButton_clicked();

    void on_MaxButton_clicked();

    void on_CloseButton_clicked();

    void on_RecoverButton_clicked();

    void on_MenuButton_clicked();

    void menuBarActionClicked(QAction* action);

private:
    void initSignal();//用于连接所有信号

    void setMouseShape(const QPoint& p);
    void setFunc();
    void setMenu();
    void addMenuTobar(QMenu* menuBar);
    QMenu* createMenu(const QStringList &actions, QWidget* parent);
    QAction* createAction(const QString &text, QWidget* parent);
    void menuBarMove();
    bool findNoButton();
    void setButton();

private:
    Ui::FramelessWidget *ui;

    QString ini_file;
    QString buttonStyle;

    QImage *img = new QImage;
    QMenuBar *menuBar;

    bool isLeftPressed;
    bool isDoubleClick;
    bool titlePressed;
    bool menuButtonPressed;
    bool edc;
    bool fixed;
    bool m_widgetPosInit = false;

    QPoint mouseLastPos;
    QPoint m_lastPos; //最大化前的窗口位置
    QPoint m_widgetPos;
    QPoint mousePos;
    QPoint wndPos;
    QPoint m_lastMousePos;

    Location location;
    Func func;

    int last_width;
    int clickCount;

    int screenWidth;
    int screenHeigh;
};

#endif // FRAMELESSWIDGET_H

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "FramelessWidget.h"
#include "filetreewidget.h"
#include "newwidget.h"
#include "codeeditor.h"
#include "debugwidget.h"
#include "highlighter.h"
#include "emulatorwidget.h"
#include "leftwidget.h"

namespace Ui {
class mainwidget;
}

class mainwidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit mainwidget(QWidget *parent = nullptr);
    ~mainwidget();

    void initFuncMenu();
    void init();
    void initFileWidgetButton();
    void initStyle();
    void initListToolButton();
    void setLeftButton();//设置左边按钮样式
    void setProButton();
    void resetProButton();

private slots:

    void leftFuncMenuSlots();
    void proButtonSlots();
    void clickedNewFileSlot();
    void newFileButtonClickedSlots(QAction* action);
    void buildFolderSlot();
    void clickFunMenuSlot(QAction* action);

private:
    QString m_file;

    Ui::mainwidget *ui;

    FileTreewidget* fileTree;
    Newwidget* newwidget;

    CodeEditor *editor;
    Highlighter *highlighter;

    Debugwidget* debugwidget;

    Leftwidget* leftwidget;
    emulatorwidget* emuwidget;

    QString style1;
    QString style2;

    QTabWidget* edit_widget;

    MyMenu* funcMenu;
    QList<QAction*> ListActions;
    QList<QToolButton*> ListToolButton;
    QWidget* menu_widget;

    void setupEditor();
    void setLayout(QWidget* widget);
    void setNewwidget(QString type, int idx);
//    void setTmpFile(QString* tmpFilePath);

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent* event) override;

    // QObject interface
public:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // MAINWIDGET_H

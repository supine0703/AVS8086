#include "titlebar.h"
#include "ui_titlebar.h"
#include <QPushButton>
#include <QDebug>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QList>
#include <QImage>
#include <QMenu>
#include <QActionGroup>
#include <QPropertyAnimation>

Titlebar::Titlebar(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::titlebar)
    , menuBar(new QMenuBar(this))
    , parent_MainWindow(parent)
{
    ui->setupUi(this);
    setMenu();
    mousePressed = false;
    last_width = 1000;

    InitDesigner();

    ui->RecoverButton->setVisible(false);

    this->setMouseTracking(true);

}

//获取窗口改变前的宽度，设置最大最小按钮时候有用
void Titlebar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        last_width = parent_MainWindow->width();
    }
}

//获取鼠标按下的位置
void Titlebar::mousePressEvent(QMouseEvent *event) {
    mousePressed = true;
    mousePos = event->globalPosition().toPoint();

    if (parent_MainWindow) wndPos = parent_MainWindow->pos();

//    if ()

}

void Titlebar::mouseMoveEvent(QMouseEvent *event) {
    //  QWidget *parent = parentWidget();
    //  if (parent) parent = parent->parentWidget();

    if (parent_MainWindow && mousePressed)
    {
        if(!parent_MainWindow->isMaximized())
        {
            //界面的全局位置 + （当前鼠标位置-鼠标按下时的位置）
            parent_MainWindow->move(wndPos + (event->globalPosition().toPoint() - mousePos));
        }
        else
        {
            //窗口最大化时鼠标拖动标题栏使窗体复原，且鼠标相对窗体的相对位置不变
            //相对位置不变指的是：鼠标点击拖动窗口1/4处进行拖动，复原时鼠标依然位于窗口1/4处
            //达到此效果，仅需更改窗口的位置即可
            float width_ratio = float(event->globalPosition().toPoint().x())/float(parent_MainWindow->width());
            on_RecoverButton_clicked();
            //更新窗体的位置，将鼠标点击的窗体标题栏的相对位置定位到鼠标当前位置
            int normal_width = last_width * width_ratio;
            int normal_higth = this->height()/4;
            wndPos = event->globalPosition().toPoint() - QPoint(normal_width,normal_higth);
        }
    }

}

void Titlebar::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    mousePressed = false;
}

void Titlebar::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);

    if (parent_MainWindow->windowState().testFlag(Qt::WindowNoState)) {
        on_MaxButton_clicked();
    }
    else{
        on_RecoverButton_clicked();
    }
}

void Titlebar::on_MiniButton_clicked()
{
    parent_MainWindow->setWindowState(Qt::WindowMinimized);
}

void Titlebar::on_RecoverButton_clicked()
{
    ui->RecoverButton->setVisible(false);
    ui->MaxButton->setVisible(true);

    parent_MainWindow->setWindowState(Qt::WindowNoState);
}

void Titlebar::on_MenuButton_clicked()
{
    if (!menuButtonPressed)
    {
        menuBar->setVisible(true);
        menuButtonPressed = true;
    }
    else
    {
        menuBar->setVisible(false);
        menuButtonPressed = false;
    }

    menuBarMove();

}

void Titlebar::setMenu()
{
    MyMenu *fileMenu = new MyMenu(tr("&file"), this);

    QStringList fileAction = {tr("new"), tr("examples"), tr("open..."), tr("save")
                            , tr("save as..."), tr("print..."), tr("export to HTML"), tr("exit")};

    QList<QAction*> fileActionList;

    for (auto L : fileAction)
    {
        fileActionList.append(createAction(L, this));
    }

    QStringList func1Actions = {tr("com template"), tr("exe template"), tr("bin template"), tr("boot template")};

    MyMenu* func1 = createMenu(func1Actions, this);

    fileActionList[0]->setMenu(func1);

    QStringList func2Actions = {tr("Hello, World"), tr("add / subtract"), tr("calculate sum"), tr("compara numbers")
        , tr("binary, hex and octal values"), tr("traffic lights"), tr("palindrome"), tr("LED display test")
                                , tr("stepper motor"), tr("simple i/o"), tr("more examples...")};

    MyMenu* func2 = createMenu(func2Actions, this);
    fileActionList[1]->setMenu(func2);


    for (auto a : fileActionList)
    {
        fileMenu->addAction(a);
    }

    fileActionList.clear();

    MyMenu *editMenu = new MyMenu(tr("&edit"), this);

    fileAction = {tr("undo"), tr("redo"), tr("cut"), tr("copy"), tr("paste"), tr("select all"), tr("find...                  Ctrl+F")
                  , tr("find next            F3"), tr("replace...            Ctrl+H"), tr("go to line           Ctrl+G"), tr("indent                Tab")
                  , tr("outdent             Shift+Tab"), tr("comment block          Ctrl+Q"), tr("uncomment block           Ctrl+W")
                  , tr("advanced editor macros"), tr("advanced")};

    for (auto L : fileAction)
    {
        fileActionList.append(createAction(L, this));
    }

    func1Actions = {tr("Play Macro 1"), tr("Play Macro 2"), tr("Play Macro 3"),tr("Play Macro 4"), tr("Play Macro 5")
                    , tr("Play Macro 6"), tr("Play Macro 7"), tr("Play Macro 8"), tr("Play Macro 9"), tr("Play Macro 10")
                    , tr("Repeat Next Command...    Ctrl+R"), tr("Record new Keystroke Macro")};

    func1 = createMenu(func1Actions, this);

    fileActionList[14]->setMenu(func1);

    func1Actions = {tr("show line numbers"), tr("tabify selection"), tr("untabify selection"), tr("lowercase selection")
                    , tr("uppercase selection"), tr(" display whitespace")};

    func2 = createMenu(func1Actions, this);

    fileActionList[15]->setMenu(func2);

    for (auto ac : fileActionList)
    {
        editMenu->addAction(ac);
    }

    MyMenu *bookmarksMenu = new MyMenu(tr("&bookmarks"), this);

    fileAction = {tr("toggle bookmark         Ctrl+F2"), tr("previous bookmark      Shift+F2")
                  , tr("next bookmark             F2"), tr("jump to first"), tr("jump to last"), tr("clear all bookmarks")};

    for (auto ac : fileAction)
    {
        bookmarksMenu->addAction(createAction(ac, this));
    }

    MyMenu *assemblerMenu = new MyMenu(tr("&assembler"), this);

    fileAction = {tr("compile"), tr("compile and load in the emulator   F5"), tr("fasm"), tr("set output directory...")};

    for (auto ac : fileAction)
    {
        assemblerMenu->addAction(createAction(ac, this));
    }

    MyMenu *emulatorMenu = new MyMenu(tr("&assembler"), this);

    fileAction = {tr("show emulator"), tr("assemble and load in the emulator")};

    for (auto ac : fileAction)
    {
        emulatorMenu->addAction(createAction(ac, this));
    }

    MyMenu *mathMenu = new MyMenu(tr("&math"), this);

    fileAction = {tr("multi base calculator"), tr("base converter")};

    for (auto ac : fileAction)
    {
        mathMenu->addAction(createAction(ac, this));
    }

    MyMenu *asciiMenu = new MyMenu(tr("&ascii codes"), this);//要写一个窗口
    MyMenu *helpMenu = new MyMenu(tr("&help"), this);

    fileAction = {tr("documentation and tutorials        F1"), tr("check for an update..."), tr(" about...")};

    for (auto ac : fileAction)
    {
        helpMenu->addAction(createAction(ac, this));
    }

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(bookmarksMenu);
    menuBar->addMenu(assemblerMenu);
    menuBar->addMenu(emulatorMenu);
    menuBar->addMenu(mathMenu);
    menuBar->addMenu(asciiMenu);
    menuBar->addMenu(helpMenu);
//    ui->MenuButton->setVisible(false);

    //设置格式
    QString menuBarsty = R"(
        QMenuBar::item {
            padding:  0 20px; // 设置间隔
            margin-top:  1px;
            margin-left:  1px;
            padding:  3px 8px;
            color:  black;
            background:  transparent;
            border-radius:  0px;
            width:  65px;
            height:  26px;
        }
    )";

    QString menusty = R"(
        "QMenu{
            border-radius: 20px;
        }"
    )";

    QList<MyMenu*> menus = menuBar->findChildren<MyMenu*>();
    QFont font;
    font.setPointSize(10);
    menuBar->setFont(font);

    menuBar->setStyleSheet(menuBarsty);

    for (auto* m : menus)
    {
        m->setFont(font);
        m->setStyleSheet(menusty);
    }


    ui->menu_widget->layout()->addWidget(menuBar);

    menuBar->setVisible(false);
}

void Titlebar::addMenuTobar(MyMenu *menu)
{
    menuBar->addMenu(menu);
}

MyMenu* Titlebar::createMenu(const QStringList &actions, QWidget *parent)
{
    MyMenu* menu = new MyMenu(parent);

    for (auto ac : actions)
    {
        menu->addAction(new QAction(ac, parent));
    }

    return menu;
}

QAction *Titlebar::createAction(const QString &text, QWidget *parent)
{
    QAction* action = new QAction(text, parent);
    return action;
}


void Titlebar::menuBarMove()
{
    QPropertyAnimation* animation = new QPropertyAnimation(menuBar, "geometry");
    animation->setStartValue(QRect(menuBar->x(), menuBar->y(), 0, menuBar->height()));
    animation->setEndValue(QRect(menuBar->x(), menuBar->y(), menuBar->width(), menuBar->height()));
    animation->setDuration(1000);
    animation->start();
}

void Titlebar::on_MaxButton_clicked()
{
    ui->RecoverButton->setVisible(true);
    ui->MaxButton->setVisible(false);

    parent_MainWindow->setWindowState(Qt::WindowMaximized);
    parent_MainWindow->showMaximized();
}

void Titlebar::on_CloseButton_clicked()
{
    parent_MainWindow->close();
}

void Titlebar::InitDesigner()
{
    ui->CloseButton->setIcon(QIcon(QPixmap(":/icon_window_close.png")));
    ui->MaxButton->setIcon(QIcon(QPixmap(":/icon_window_maximize.png")));
    ui->MiniButton->setIcon(QIcon(QPixmap(":/icon_window_minimize.png")));
    ui->RecoverButton->setIcon(QIcon(QPixmap(":/icon_window_restore.png")));
    ui->MenuButton->setIcon(QIcon(QPixmap(":/menu.png")));

    buttonStyle = R"(
        QPushButton {
            border: none;
    }
        QPushButton:hover {
            background-color: rgb(128, 128, 128);
    }
        QPushButton:pressed {
            background-color: rgb(110, 110, 110);
    }
)";

    ui->MiniButton->setStyleSheet(buttonStyle);
    ui->RecoverButton->setStyleSheet(buttonStyle);
    ui->MaxButton->setStyleSheet(buttonStyle);
    ui->CloseButton->setStyleSheet(buttonStyle);
    ui->MenuButton->setStyleSheet(buttonStyle);

}

Titlebar::~Titlebar()
{
    delete ui;
}


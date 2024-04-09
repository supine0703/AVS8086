#include "FramelessWidget.h"
#include "ui_FramelessWidget.h"
#include "settings.h"
#include <QPushButton>
#include <QDebug>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QDebug>
#include <QPainter>
#include <QList>
#include <QImage>
#include <QMenu>
#include <QActionGroup>
#include <QPropertyAnimation>
#include <QSettings>


#define PADDING 5 //5个像素

FramelessWidget::FramelessWidget(QWidget *parent, Func func_)
    : QDialog(parent)
    , isLeftPressed(false)
    , titlePressed(false)
    , menuButtonPressed(false)
    , edc(false)
    , fixed(false)
    , isDoubleClick(false)
    , menuBar(new QMenuBar(this))
    , func(func_)
    , last_width(1000)
    , ui(new Ui::FramelessWidget)
{
    ui->setupUi(this);

    InitDesigner();

    setFunc();

    QApplication::instance()->installEventFilter(this);

    //去除标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
    windowShadow->setBlurRadius(9.0);

    setMenu();

    screenWidth =  QGuiApplication::screenAt(QCursor::pos())->geometry().width();
    screenHeigh = QGuiApplication::screenAt(QCursor::pos())->geometry().height();

    connect(menuBar, &QMenuBar::triggered, this, &FramelessWidget::menuBarActionClicked);//点击menuBar事件

//    connect(_clickTimer, &QTimer::timeout, this, &FramelessWidget::clickTimerSlot);

    ui->RecoverButton->setVisible(false);
//    ui->main_widget->layout()->addWidget(childwidget);
    this->setMouseTracking(true);

    if (SETTINGS_UI().contains(_WINDOW_SIZE_))
        this->resize(SETTINGS_UI().value(_WINDOW_SIZE_).toSize());
    if (SETTINGS_UI().contains(_WINDOW_POS_))
        this->move(SETTINGS_UI().value(_WINDOW_POS_).toPoint());


    connect(ui->MenuButton, &QToolButton::clicked, this, [this](){
       emit menuButtonClicked();
    });
}

//获取窗口改变前的宽度，设置最大最小按钮时候有用
void FramelessWidget::changeEvent(QEvent *event)
{
    QWindowStateChangeEvent* stateChangeEvent = static_cast<QWindowStateChangeEvent*>(event);

    if (event->type() == QEvent::WindowStateChange)
    {
        last_width = this->width();
        if (stateChangeEvent->oldState() & Qt::WindowMaximized)
        {

        }
        else if (stateChangeEvent->oldState() != Qt::WindowMinimized)
        {
            SETTINGS_UI().setValue(_WINDOW_SIZE_, this->size());
        }
//        if (stateChangeEvent->oldState() & Qt::WindowMaximized)
//        {
//            this->showNormal();
//            QPoint newPos = m_lastPos + (this->pos())
//        }
//        if (stateChangeEvent->oldState() & !isMaximized())
//        {
//            QSettings s(CONFIG_INI, QSettings::IniFormat);
//            s.setValue("/main_window/not_max_size", this->size());
//        }
    }
}

void FramelessWidget::setFunc()
{
    switch(func)
    {
    case Max:
        ui->CloseButton->setVisible(false);
        ui->MiniButton->setVisible(false);
        ui->MenuButton->setVisible(false);
        break;
    case Min:
        ui->CloseButton->setVisible(false);
        ui->MaxButton->setVisible(false);
        ui->MenuButton->setVisible(false);
        break;
    case Close:
        ui->MaxButton->setVisible(false);
        ui->MiniButton->setVisible(false);
        ui->MenuButton->setVisible(false);
        ui->menu_widget->setVisible(false);
        break;
    case All:
        break;
    default:
        break;
    }
}

FramelessWidget::~FramelessWidget()
{
    delete ui;
}

void FramelessWidget::mousePressEvent(QMouseEvent *event)
{   
    mousePos = event->globalPosition().toPoint();
    wndPos = this->pos();

    switch (event->button())
    {
    case Qt::RightButton:
        //        this->close();
        break;
    case Qt::LeftButton:
        isLeftPressed = true;
        if (this->mapFromGlobal(mousePos).y() <= 30 && this->mapFromGlobal(mousePos).y() >= 0)
        {
            titlePressed = true;
//            mouseLastPos = event->pos();
            mouseLastPos = event->globalPosition().toPoint();
//            mouseOffset = event->globalPosition().toPoint() - this->frameGeometry().topLeft();
//            qDebug() << event->pos();
//            qDebug() << mouseOffset;
//            qDebug() << "press:" << mouseLastPos;
        }
        else
        {
            if (menuButtonPressed)
            {
                on_MenuButton_clicked();
            }
        }
    default:
        QWidget::mousePressEvent(event);
        break;
    }
}

void FramelessWidget::mouseMoveEvent(QMouseEvent  *event)
{
    QPoint globalPos = event->globalPosition().toPoint();

    //未按下
    if (!isLeftPressed && !fixed && !isMaximized())
    {
        this->setMouseShape(globalPos);
        return;
    }

    if (fixed)//当设置fixed时，使得鼠标图标不再变化
    {
        location = CENTER;
    }


    QRect rect = this->rect();
    QPoint topLeft = mapToGlobal(rect.topLeft());//0, 0
    QPoint bottomRight = mapToGlobal(rect.bottomRight());

    if (!this->isMaximized() && !fixed && location != CENTER)
    {
        //缩放
        QRect rMove(topLeft, bottomRight);

        switch (location)
        {
        case TOP:
            if ((bottomRight.y() - globalPos.y()) > this->minimumHeight())//防止缩放导致窗口偏移
            {
                rMove.setTop(globalPos.y());
            }
            break;
        case BOTTOM:
            rMove.setBottom(globalPos.y());//不加是因为其实现原理是改变Height,被minisize限制了
            break;
        case LEFT:
            if ((bottomRight.x() - globalPos.x()) > this->minimumWidth())//防止缩放导致窗口偏移
            {
                rMove.setLeft(globalPos.x());
            }
            break;
        case RIGHT:
            rMove.setRight(globalPos.x());
            break;
        case TOP_LEFT:
            if ((bottomRight.y() - globalPos.y()) > this->minimumHeight())//防止缩放导致窗口偏移
            {
                rMove.setTop(globalPos.y());
            }
            if ((bottomRight.x() - globalPos.x()) > this->minimumWidth())//防止缩放导致窗口偏移
            {
                rMove.setLeft(globalPos.x());
            }
            break;
        case TOP_RIGHT:
            if ((bottomRight.y() - globalPos.y()) > this->minimumHeight())//防止缩放导致窗口偏移
            {
                rMove.setTop(globalPos.y());
            }
            rMove.setRight(globalPos.x());
            break;
        case BOTTOM_LEFT:
            rMove.setBottom(globalPos.y());
            if ((bottomRight.x() - globalPos.x()) > this->minimumWidth())//防止缩放导致窗口偏移
            {
                rMove.setLeft(globalPos.x());
            }
            break;
        case BOTTOM_RIGHT:
            rMove.setBottom(globalPos.y());
            rMove.setRight(globalPos.x());
            break;
        default:
            break;
        }

        this->setGeometry(rMove);
        SETTINGS_UI().setValue(_WINDOW_SIZE_, this->size());
        m_widgetPos = this->pos();
        SETTINGS_UI().setValue(_WINDOW_POS_, this->pos());
    }

    if (titlePressed)
    {
        if(!this->isMaximized() && location != TOP && location != TOP_RIGHT && location != TOP_LEFT)
        {
            //界面的全局位置 + （当前鼠标位置-鼠标按下时的位置
            // 初始化
            if (!m_widgetPosInit)
            {
                m_widgetPosInit = true;
                m_widgetPos = this->pos();
            }

            auto mouseNowPos =  event->globalPosition().toPoint();
            m_widgetPos += mouseNowPos - mouseLastPos;
            mouseLastPos = mouseNowPos;
            this->move(m_widgetPos);


            event->accept();
            return;
        }
        else
        {
            if(this->isMaximized() && location != TOP && location != TOP_RIGHT && location != TOP_LEFT)
            {
                this->resize(SETTINGS_UI().value(_WINDOW_SIZE_).toSize());

                QPoint leftTopPos =
                    (globalPos.x() > screenWidth / 2)
                    ? QPoint(screenWidth - this->width(), 0)    // right
                    : QPoint(0, 0);                             // left

                // if mouse not in width
                if (!(leftTopPos.x() <= globalPos.x() && globalPos.x() <= leftTopPos.x() + this->width()))
                {
                    leftTopPos.setX(globalPos.x() - this->width() / 2);
                }

                m_widgetPos = leftTopPos;

                ui->RecoverButton->setVisible(false);
                ui->MaxButton->setVisible(true);
            }
        }
    }




    QWidget::mouseMoveEvent(event);
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (event->button() == Qt::LeftButton)
    {
        isLeftPressed = false;
        titlePressed = false;
    }
    else
        QWidget::mouseReleaseEvent(event);
}

bool FramelessWidget::eventFilter(QObject *obj, QEvent *event)
{
    QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);


//    qDebug()<<isLeftPressed<<titlePressed<<isMaximized();

    // if (isMaximized() && isLeftPressed)
    // {
    //     return QWidget::eventFilter(obj, event);
    // }

    // check mouse move event when mouse is moved on any object
    if (event->type() == QEvent::MouseMove)
    {
        if (pMouse) {
            mouseMoveEvent(pMouse);
        }
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        mousePressEvent(pMouse);
        return QWidget::eventFilter(obj, event);
    }

    else if (event->type() == QEvent::MouseButtonDblClick  && !fixed)
    {
        qDebug()<<"000";
        if (isLeftPressed && titlePressed)
        {
            if (!isMaximized())
            {
                qDebug()<<"111";
                on_MaxButton_clicked();
                return true;
            }
            else
            {
                qDebug()<<"222";
                on_RecoverButton_clicked();
                return true;
            }
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if (isLeftPressed) {
            pMouse = dynamic_cast<QMouseEvent *>(event);
            if (pMouse) {
                mouseReleaseEvent(pMouse);
            }
        }
    } /*else if (){}*/

    return QWidget::eventFilter(obj, event);
}


void FramelessWidget::setMouseShape(const QPoint& point)
{
    QRect rect = this->rect();
    QPoint topLeft = mapToGlobal(rect.topLeft());
    QPoint boRight = mapToGlobal(rect.bottomRight());

    int x = point.x();
    int y = point.y();

    //左上角
    if (x >= topLeft.x() && x <= topLeft.x() + PADDING &&
        y >= topLeft.y() && y <= topLeft.y() + PADDING)
    {
        location = TOP_LEFT;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }//右上角
    else if (x <= boRight.x() && x >= boRight.x() - PADDING &&
             y >= topLeft.y() && y <= topLeft.y() + PADDING)
    {
        location = TOP_RIGHT;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }//左下角
    else if (x >= topLeft.x() && x <= topLeft.x() + PADDING &&
             y <= boRight.y() && y >= boRight.y() - PADDING)
    {
        location = BOTTOM_LEFT;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }//右下角
    else if (x <= boRight.x() && x >= boRight.x() - PADDING &&
             y <= boRight.y() && y >= boRight.y() - PADDING)
    {
        location = BOTTOM_RIGHT;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    }//左
    else if (x >= topLeft.x() && x <= topLeft.x() + PADDING)
    {
        location = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }//右
    else if (x <= boRight.x() && x >= boRight.x() - PADDING)
    {
        location = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }//上
    else if (y >= topLeft.y() && y <= topLeft.y() + PADDING)
    {
        location = TOP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }//下
    else if (y <= boRight.y() && y >= boRight.y() - PADDING)
    {
        location = BOTTOM;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }//中间
    else
    {
        location = CENTER;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

void FramelessWidget::on_MiniButton_clicked()
{
    this->setWindowState(Qt::WindowMinimized);
}

void FramelessWidget::on_RecoverButton_clicked()
{
    ui->RecoverButton->setVisible(false);
    ui->MaxButton->setVisible(true);

    this->setWindowState(Qt::WindowNoState);
}

void FramelessWidget::on_CloseButton_clicked()
{
    if (location != TOP && location != TOP_RIGHT)
        this->close();
}

void FramelessWidget::on_MenuButton_clicked()
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

void FramelessWidget::menuBarActionClicked(QAction* action)
{
    //{tr("com template"), tr("exe template"), tr("bin template"), tr("boot template")};
    if (action->text() == "com template" || action->text() == "exe template"
        || action->text() == "bin template"|| action->text() == "boot template")
    {
        emit newFileButtonClicked(action);
    }
//    if () 其他方法
}

QWidget* FramelessWidget::getMenuwidget()
{
    return ui->menu_widget;
}

void FramelessWidget::setWidgetFixed(int w, int h)
{
    this->setFixedSize(w, h);
    fixed = true;
}

void FramelessWidget::setMenu()
{
    MyMenu *fileMenu = new MyMenu(tr("&file"), this);

    QStringList fileAction = {tr("new"), tr("examples"), tr("open..."), tr("save")
                              , tr("save as..."), tr("print..."), tr("export to HTML"), tr("exit")};

    QList<QAction*> fileActionList;

    for (const auto& L : fileAction)
    {
        fileActionList.append(createAction(L, this));
    }

    QStringList func1Actions = {tr("com template"), tr("exe template"), tr("bin template")};

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

    for (const auto& L : fileAction)
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

    for (const auto& ac : fileActionList)
    {
        editMenu->addAction(ac);
    }

    MyMenu *bookmarksMenu = new MyMenu(tr("&bookmarks"), this);

    fileAction = {tr("toggle bookmark         Ctrl+F2"), tr("previous bookmark      Shift+F2")
                  , tr("next bookmark             F2"), tr("jump to first"), tr("jump to last"), tr("clear all bookmarks")};

    for (const auto& ac : fileAction)
    {
        bookmarksMenu->addAction(createAction(ac, this));
    }

    MyMenu *assemblerMenu = new MyMenu(tr("&assembler"), this);

    fileAction = {tr("compile"), tr("compile and load in the emulator   F5"), tr("fasm"), tr("set output directory...")};

    for (const auto& ac : fileAction)
    {
        assemblerMenu->addAction(createAction(ac, this));
    }

    MyMenu *emulatorMenu = new MyMenu(tr("&assembler"), this);

    fileAction = {tr("show emulator"), tr("assemble and load in the emulator")};

    for (const auto& ac : fileAction)
    {
        emulatorMenu->addAction(createAction(ac, this));
    }

    MyMenu *mathMenu = new MyMenu(tr("&math"), this);

    fileAction = {tr("multi base calculator"), tr("base converter")};

    for (const auto& ac : fileAction)
    {
        mathMenu->addAction(createAction(ac, this));
    }

    MyMenu *asciiMenu = new MyMenu(tr("&ascii codes"), this);//要写一个窗口
    MyMenu *helpMenu = new MyMenu(tr("&help"), this);

    fileAction = {tr("documentation and tutorials        F1"), tr("check for an update..."), tr(" about...")};

    for (const auto& ac : fileAction)
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
            color: white;
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

void FramelessWidget::addMenuTobar(MyMenu *menu)
{
    menuBar->addMenu(menu);
}

MyMenu* FramelessWidget::createMenu(const QStringList &actions, QWidget *parent)
{
    MyMenu* menu = new MyMenu(parent);

    for (const auto& ac : actions)
    {
        menu->addAction(new QAction(ac, parent));
    }

    return menu;
}

QAction *FramelessWidget::createAction(const QString &text, QWidget *parent)
{
    QAction* action = new QAction(text, parent);
    return action;
}

void FramelessWidget::menuBarMove()
{
    QPropertyAnimation* animation = new QPropertyAnimation(menuBar, "geometry");
    animation->setStartValue(QRect(menuBar->x(), menuBar->y(), 0, menuBar->height()));
    animation->setEndValue(QRect(menuBar->x(), menuBar->y(), menuBar->width(), menuBar->height()));
    animation->setDuration(1000);
    animation->start();
}

//bool FramelessWidget::findNoButton()
//{
//    QPoint globalPos = QCursor::pos();
//    QPoint ClosePos = ui->CloseButton->pos();
//    QPoint MaxPos = ui->MaxButton->pos();
//    QPoint MiniPos = ui->MiniButton->pos();
//    QPoint RecPos = ui->RecoverButton->pos();


//}

//void FramelessWidget::setButton()
//{
//    QPoint globalPos = this->mapToGlobal(QCursor::pos());

//    QPoint ClosePos = this->mapToGlobal(ui->CloseButton->pos());
//    QPoint MaxPos = this->mapToGlobal(ui->MaxButton->pos());
//    QPoint MiniPos = this->mapToGlobal(ui->MiniButton->pos());
//    QPoint RecPos = this->mapToGlobal(ui->RecoverButton->pos());

//    if (globalPos.y() <= 3 && globalPos.y() >= 0
//        && globalPos.x() - ClosePos.x() >= 33 && globalPos.x() - ClosePos.x() <= 36)
//    {
//        ui->CloseButton->setAttribute((Qt::WA_TransparentForMouseEvents));
//    }
////    if (MaxRect.contains(MaxPos))
////    {
////    }
////    else
////    {
////    }
////    if (MiniRect.contains(MiniPos))
////    {
////    }
////    else
////    {
////    }
////    if (RecRect.contains(RecPos))
////    {
////    }
////    else
////    {
//    //    }
//}

void FramelessWidget::setcentralWidget(QWidget *widget, Func func = All)
{
    if (widget == nullptr)
        widget = new QWidget;

    FramelessWidget* framelesswidget = new FramelessWidget(widget->parentWidget(), func);
    widget->setParent(framelesswidget);
    framelesswidget->ui->main_widget->deleteLater();
    framelesswidget->ui->centrallayout->addWidget(widget);

}

QLabel* FramelessWidget::getmenu_label()
{
    return ui->menu_label;
}

QWidget* FramelessWidget::getMain_Widget()
{
    return ui->main_widget;
}


void FramelessWidget::on_MaxButton_clicked()
{
    ui->RecoverButton->setVisible(true);
    ui->MaxButton->setVisible(false);

    this->setWindowState(Qt::WindowMaximized);
    this->showMaximized();
}


void FramelessWidget::InitDesigner()
{
    ui->CloseButton->setIcon(QIcon(":/icon_window_close.png"));
    ui->MaxButton->setIcon(QIcon(":/icon_window_maximize.png"));
    ui->MiniButton->setIcon(QIcon(":/icon_window_minimize.png"));
    ui->RecoverButton->setIcon(QIcon(":/icon_window_restore.png"));
    ui->MenuButton->setIcon(QIcon(":/menu.png"));
    ui->toolButton->setIcon(QIcon(":/cpu.png"));

    buttonStyle = R"(
        QToolButton {
            border: none;
    }
        QToolButton:hover {
            background-color: rgb(128, 128, 128);
    }
        QToolButton:pressed {
            background-color: rgb(110, 110, 110);
    }
)";

    ui->MiniButton->setStyleSheet(buttonStyle);
    ui->RecoverButton->setStyleSheet(buttonStyle);
    ui->MaxButton->setStyleSheet(buttonStyle);
    ui->CloseButton->setStyleSheet(buttonStyle);
    ui->MenuButton->setStyleSheet(buttonStyle);

    QPainter painter(this);
    painter.setPen(QPen(Qt::gray, 1));
    painter.drawRect(ui->Frameless_widget->rect()); // 绘制矩形描边

//    ui->Frameless_widget->setStyleSheet("border: 1px solid gray;");
}

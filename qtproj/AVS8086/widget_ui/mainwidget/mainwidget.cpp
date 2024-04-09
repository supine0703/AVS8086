#include "QVBoxLayout"
#include "settings.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QMessageBox>
#include <QSplitter>
#include <QTableWidget>

mainwidget::mainwidget(QWidget *parent) :
    FramelessWidget("main", parent, Func::All)
    , fileTree(new FileTreewidget(this))
    , newwidget(new Newwidget())
    , edit_widget(new QTabWidget())
    , leftwidget(new Leftwidget(this))
    , editor(new CodeEditor(this))
    , debugwidget(new Debugwidget(this))
    , funcMenu(new MyMenu(this))
    , emuwidget(new emulatorwidget(this))
    , menu_widget(FramelessWidget::getMenuwidget())
    , highlighter(new Highlighter(editor->document()))
    , ui(new Ui::mainwidget)
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();
    // Settings::window = main_widget;

    ui->setupUi(main_widget);
    this->ui->dirwidget->resize(200, this->height() - 30);
    newwidget->setModal(true);
    main_widget->layout()->addWidget(ui->main_widget);

    fileTree->setParent(ui->dirwidget);

    ui->show_label->setVisible(false);

    init();

    qDebug()<<debugwidget->getErrlineList()->isEmpty();

    editor->getWarnLineList(debugwidget->getWarnlineList());
    editor->getErrLineList(debugwidget->getErrlineList());

    // connect(editor->document(), &QTextDocument::errLineWordChanged, this, [this](){
    //     editor->warnLineWordChangedReicive(debugwidget->getWarnlineList());
    // });

    connect(editor, &CodeEditor::ssssss, this, [this](){

        debugwidget->warnStringList = editor->warnLineStringList;
        debugwidget->errStringList = editor->errLineStringList;
        debugwidget->add_();
    });//传递错误的单词列

    connect(fileTree, &FileTreewidget::openFileSignals, editor, &CodeEditor::openFileSignalsSlot);
    connect(fileTree, &FileTreewidget::saveFile, editor, &CodeEditor::saveFileSlot);
    connect(ui->closebutton, &QToolButton::clicked, this, [this](){
        ui->widget_5->setVisible(false);
        for (auto button : ListToolButton)
        {
            button->setStyleSheet(style1);
        }
        ui->closebutton->setStyleSheet(style1);
    });

    connect(newwidget, &Newwidget::PathEditContent, fileTree, &FileTreewidget::newProject);

    connect(fileTree, &FileTreewidget::nowFilePath, this, [this](auto f) {
        m_file = f;
    });

    connect(leftwidget, &Leftwidget::runButton_clicked, emuwidget, [this]() {
        emuwidget->setFile(m_file);
        emuwidget->runButton_clickedSlot();
    });
    connect(fileTree, &FileTreewidget::nowFilePath, editor, &CodeEditor::setCompileFile);
}

mainwidget::~mainwidget()
{   
    delete ui;
}

void mainwidget::initFuncMenu()
{
    ListActions += new QAction(tr("new"));
    ListActions += new QAction(tr("open"));
    ListActions += new QAction(tr("examples"));
    ListActions += new QAction(tr("save"));
    ListActions += new QAction(tr("calculator    "));
    ListActions += new QAction(tr("convertor"));
    ListActions += new QAction(tr("options"));
    ListActions += new QAction(tr("help"));

    for (auto action : ListActions)
    {
        funcMenu->addAction(action);
    }

    funcMenu->setStyleSheet("color:rgb(239, 239, 239);"
                            "background-color:rgb(50, 51, 52)");
    connect(funcMenu, &QMenu::triggered, this, &mainwidget::clickFunMenuSlot);

}

void mainwidget::init()
{
    //    QString("%1").arg(1, 4, 10, QChar(' '));
    ui->leftwidget->layout()->addWidget(leftwidget);
    ui->dirwidget->layout()->addWidget(fileTree);
    ui->widget_4->layout()->addWidget(editor);
    ui->showerr_widget->layout()->addWidget(debugwidget);
    ui->filetreewidget->setVisible(false);

    // 假设splitter是您的QSplitter对象
    ui->splitter->setStyleSheet("QSplitter::handle { background-color: rgb(37, 37, 38); }");
    ui->splitter_2->setStyleSheet("QSplitter::handle { background-color: rgb(37, 37, 38); }");
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter_2->setChildrenCollapsible(false);
    ui->splitter->setSizes({this->width()/10, this->width()/10*9});
    ui->splitter_2->setSizes({this->height()/3*2, this->height()/3*1});

//    buildFileWidget->setLabelText("新建文件");
//    buileDirWidget->setLabelText("新建文件夹");

    this->ui->dirwidget->resize(200, this->height() - 30);

    initFuncMenu();
    initStyle();
    initListToolButton();
    initFileWidgetButton();

    ui->closebutton->setIcon(QIcon(":/jianhao.png"));

    this->setMinimumSize(600, 400);

    connect(leftwidget, &Leftwidget::funcButton_clicked, this, &mainwidget::leftFuncMenuSlots);
    connect(leftwidget, &Leftwidget::proButton_clicked, this, &mainwidget::proButtonSlots);

    connect(this, &FramelessWidget::newFileButtonClicked, this, &mainwidget::newFileButtonClickedSlots);

    // connect(newwidget, &Newwidget::buildFile, fileTree, &FileTreewidget::bulidNewFile);

    connect(debugwidget, &Debugwidget::errLineNumber, this, [this](int errLine, int errWord, int errWordLengh){
        editor->goToErrLine(errLine, errWord, errWordLengh);
    });//接收debug信号

    connect(fileTree, &FileTreewidget::showTextSignal, editor, &CodeEditor::showTextSignalSlot);

    connect(newwidget, &Newwidget::createAsmChecked, fileTree, &FileTreewidget::createAsmCheckedSlots);
}

void mainwidget::initFileWidgetButton()
{
    for (auto button : ListToolButton)
    {
        button->setStyleSheet(style1);
        connect(button, &QToolButton::clicked, this, [this, button](){
            resetProButton();
            button->setStyleSheet(style2);
        });
    }
}

void mainwidget::initStyle()
{
    style1 = R"(
        QToolButton:hover{
            background-color:rgb(77, 80, 86);
    }
        QToolButton{
        border:none;
        border-radius:7px;
    }
)";

    style2 = R"(
        QToolButton{
        border:none;
        background-color:rgb(53, 116, 240);
        border-radius:7px;
    }
)";
}

void mainwidget::initListToolButton()
{
    ListToolButton = ui->prowidget->findChildren<QToolButton*>();
}

void mainwidget::setLeftButton()//离开窗口时重置
{
    QPoint globalPos = QCursor::pos();//设置左边按钮

    QRect rect = ui->leftwidget->rect();

    QPoint mousePos = ui->leftwidget->mapFromGlobal(globalPos);

    if (!rect.contains(mousePos))
    {
        leftwidget->initButton();
    }
}
//这两个可以写一个方法，暂时没时间写
void mainwidget::setProButton()
{
    QPoint globalPos = QCursor::pos();//设置左边按钮

    QRect rect = ui->prowidget->rect();

    QPoint mousePos = ui->prowidget->mapFromGlobal(globalPos);

    if (!rect.contains(mousePos))
    {
        resetProButton();
    }
}

void mainwidget::resetProButton()
{
    for (auto button : ListToolButton)
    {
        button->setStyleSheet(style1);
    }
}

void mainwidget::leftFuncMenuSlots()
{
    QToolButton* funcButton = leftwidget->getFuncButton();
    QRect rect = funcButton->rect();
    QPoint globalPos = funcButton->mapToGlobal(rect.topRight());

    // 创建并发送一个鼠标离开事件要不然会有bug
    QEvent leaveEvent(QEvent::Leave);
    QApplication::sendEvent(funcButton, &leaveEvent);

    funcMenu->exec(globalPos);
}

void mainwidget::proButtonSlots()
{
    if (ui->widget_5->isVisible())
    {
        ui->widget_5->setVisible(false);
    }
    else
    {
        ui->widget_5->setVisible(true);
    }
}

void mainwidget::clickedNewFileSlot()
{

}

void mainwidget::newFileButtonClickedSlots(QAction* action)
{//{tr("com template"), tr("exe template"), tr("bin template"), tr("boot template")};
    if (action->text() == "com template")
    {
        setNewwidget("com template", 1);
    }
    else if (action->text() == "exe template")
    {
        setNewwidget("exe template", 2);
    }
    else if (action->text() == "bin template")
    {
        setNewwidget("bin template", 0);
    }
    else
    {
        QMessageBox::critical(this, "警告", "创建文件不合法！");
    }
}

void mainwidget::buildFolderSlot()
{
//    buileDirWidget->show();
//    QLineEdit* edit = buileDirWidget->getLineEdit();

//    connect(edit, &QLineEdit::editingFinished, this, [this, edit](){
//        emit
//        edit->setText("");
//        buileDirWidget->hide();
    //    })
}

void mainwidget::clickFunMenuSlot(QAction* action)
{
    //暂时就写new
    if (action->text() == "new")
    {
        setNewwidget("com template", 1);
    }
}

void mainwidget::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor->setFont(font);
}

void mainwidget::setLayout(QWidget* widget)
{
    widget->setLayout(new QVBoxLayout);
    widget->setContentsMargins(0,0,0,0);//设置间隔
}

void mainwidget::setNewwidget(QString type, int idx)
{
    QTableWidget* tablewidget = newwidget->getTableWidget();
    QTableWidgetItem *selectedItem = tablewidget->item(idx, 0);
    tablewidget->setCurrentItem(selectedItem);
    newwidget->editClear();
    newwidget->show();
}

void mainwidget::mousePressEvent(QMouseEvent* event)
{
    setLeftButton();//设置左边按钮的样式
    setProButton();

    FramelessWidget::mousePressEvent(event);
}

bool mainwidget::eventFilter(QObject* watched, QEvent* event)
{
    QMouseEvent* qMouse = dynamic_cast<QMouseEvent*>(event);

    return FramelessWidget::eventFilter(watched, event);
}

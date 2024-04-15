#include "QVBoxLayout"
#include "settings.h"
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QMessageBox>
#include <QListView>
#include <QSplitter>
#include <QFileDialog>
#include <QTableWidget>

mainwidget::mainwidget(QWidget *parent) :
    FramelessWidget("main", parent, Func::All)
    , fileTree(new FileTreewidget(this))
    , newwidget(new Newwidget())
    , edit_widget(new QTabWidget())
    , m_file(" ")
    , leftwidget(new Leftwidget(this))
    , currentFile(new QStringList())
    , currentEditor()
    , debugwidget(new Debugwidget(this))
    , funcMenu(new QMenu(this))
    , emuwidget(new emulatorwidget())
    , menu_widget(FramelessWidget::getMenuwidget())
    , ui(new Ui::mainwidget)
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();
    // Settings::window = main_widget;

    ui->setupUi(main_widget);
    this->ui->dirwidget->resize(200, this->height() - 30);
    newwidget->setModal(true);
    main_widget->layout()->addWidget(ui->main_widget);

    fileTree->setParent(ui->dirwidget);

    init();

    // qDebug()<<debugwidget->getErrlineList()->isEmpty();

    // connect(editor->document(), &QTextDocument::errLineWordChanged, this, [this](){
    //     editor->warnLineWordChangedReicive(debugwidget->getWarnlineList());
    // });

    connect(fileTree, &FileTreewidget::openFileSignals, this, [this](QString path){
        QFileInfo info = QFileInfo(path);
        if (!checkInTab(info.fileName()))
        {
            currentEditor = addNewEditor();
            currentEditor->openFileSignalsSlot(path);

            int index = ui->tabWidget->addTab(currentEditor, info.fileName());
            currentEditor->setObjectName(info.fileName());
            ui->tabWidget->setCurrentIndex(index);

            ui->tabWidget->setVisible(true);
            ui->show_label->setVisible(false);
        }

        connect(currentEditor, &CodeEditor::debugSignal, this, [this](){

            debugwidget->warnStringList = currentEditor->warnLineStringList;
            debugwidget->errStringList = currentEditor->errLineStringList;
            debugwidget->add_();
        });//传递错误的单词列

        currentEditor->getWarnLineList(debugwidget->getWarnlineList());
        currentEditor->getErrLineList(debugwidget->getErrlineList());
        currentEditor->setCompileFile(path);
    });
    connect(fileTree, &FileTreewidget::saveFile, currentEditor, &CodeEditor::saveFileSlot);

    connect(ui->closebutton, &QToolButton::clicked, this, [this](){
        ui->widget_5->setVisible(false);
        for (auto button : ListToolButton)
        {
            button->setStyleSheet(style1);
        }
        ui->closebutton->setStyleSheet(style1);
    });

    connect(ui->tabWidget, &QTabWidget::tabCloseRequested, this, [this](int index){
        auto widget = ui->tabWidget->widget(index);
        ui->tabWidget->removeTab(index);
        if(widget != nullptr){
            widget->deleteLater(); // 安全删除QWidget对象
        }
    });

    connect(newwidget, &Newwidget::PathEditContent, fileTree, &FileTreewidget::newProject);//传递创建文件的路径，用于创建文件树

    connect(fileTree, &FileTreewidget::nowFilePath, this, [this](auto f) {
        m_file = f;
    });

    connect(leftwidget, &Leftwidget::runButton_clicked, emuwidget, [this]() {
        emuwidget->setFile(m_file);
        emuwidget->runButton_clickedSlot();
    });

    connect(this, &FramelessWidget::openFileButtonClicked, this, &mainwidget::openFileButtonClickedSlot);//接收打开文件信号
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
    ui->tabWidget->setVisible(false);
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->clear();
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
        currentEditor->goToErrLine(errLine, errWord, errWordLengh);
    });//接收debug信号

    connect(fileTree, &FileTreewidget::showTextSignal, currentEditor, &CodeEditor::showTextSignalSlot);

    connect(newwidget, &Newwidget::createAsmChecked, fileTree, &FileTreewidget::createAsmCheckedSlots);

    connect(fileTree, &FileTreewidget::closeFileTree, this, [this](){
        ui->tabWidget->setVisible(false);
        ui->show_label->setVisible(true);
        ui->tabWidget->clear();
        this->debugwidget->clearAll();
    });
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

void mainwidget::openFileButtonClickedSlot()
{
    QString deFilePath;
    if (m_file == " ") deFilePath = "/home";
    else deFilePath = m_file;

    QString filePath = QFileDialog::getOpenFileName(this
                                                    , tr("打开项目")
                                                    , m_file
                                                    , "*.txt");

    QFile ConFile(filePath);
    if (!ConFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件进行读取";
        return;
    }
    QTextStream in(&ConFile);
    QString address = in.readLine();
    ConFile.close();

    fileTree->openFileSlot(address);
}

void mainwidget::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    currentEditor->setFont(font);
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

CodeEditor* mainwidget::addNewEditor()
{
    CodeEditor* editor = new CodeEditor(this);
    return editor;
}

bool mainwidget::checkInTab(QString fileName)
{
    foreach (QWidget *widget, ui->tabWidget->findChildren<QWidget*>()) {
        if (QFileInfo(widget->objectName()).fileName() == fileName) {

            return true;
        }
    }
    return false;
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

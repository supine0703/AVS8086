#include "filetreewidget.h"
#include "ui_filetreewidget.h"
#include <QLineEdit>
#include <QLineEdit>
#include <QLabel>
#include <QMenu>
#include <QProcess>
#include <QHeaderView>
#include <QMessageBox>
#include <QScrollBar>
#include <QTextStream>
// #include <fstream>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QFile>
#include <QFileDialog>

using namespace std;

FileTreewidget::FileTreewidget(QWidget *parent)
    : QTreeWidget(parent)
    , showWidget(this)
    , menubar(new QMenuBar(this))
    , dirMenu(new QMenu(this))
    , rootMenu(new QMenu(this))
    , fileMenu(new QMenu(this))
    , isChecked(false)
    , ui(new Ui::filewidget)
{
    // QString* path_ = new QString("D:\\FramelessWindow");

    // CreateTopItem(path_);

    ui->setupUi(this);

    showWidget->setObjectName("TreeMenu");//设置widget名称

    this->verticalScrollBar()->setObjectName("TreeMenuVerticl");//设置横向滑块
    this->horizontalScrollBar()->setObjectName("TreeMenuHorizontal");//设置垂直滑块
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setColumnCount(1);//设置QTreeWidget的列数为1
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);//以像素为单位滑动
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//设置为需要时才显示

    this->setAutoScroll(true);//当QTreeWidget满的时候自动滚动
    this->header()->setVisible(false);   //隐藏横向表头

    this->setFrameStyle(QFrame::Sunken);//边框呈现稍微凹陷或浮雕的外观
    this->setAnimated(true); //展开折叠动画

    //    connect(ui->pushButton, &QPushButton::clicked, this, &FileTreewidget::showNewBuildfolderWidget);

    // 创建右击文件夹的菜单栏
    dirMenu->addAction("新建文件");
    dirMenu->addAction("添加文件");
    dirMenu->addSeparator();//添加分割线
    dirMenu->addAction("新建文件夹      ");
    dirMenu->addAction("删除文件夹");
    dirMenu->addSeparator();
    dirMenu->addAction("移除文件夹");
    dirMenu->addSeparator();

    setBuildwidget();//设置创建窗口

    //创建右击rootItem
    rootMenu->addAction("关闭项目");

    // 创建右击文件的菜单栏
    fileMenu->addAction("打开文件");
    fileMenu->addAction("重命名");
    fileMenu->addAction("保存文件");
    fileMenu->addAction("移除文件");
    fileMenu->addAction("删除文件");
    fileMenu->addAction("打开所在文件夹");

    connect(dirMenu, &QMenu::triggered, this, &FileTreewidget::tempActionInformation);//右击
    connect(fileMenu, &QMenu::triggered, this, &FileTreewidget::tempActionInformation);//右击
    connect(rootMenu, &QMenu::triggered, this, &FileTreewidget::tempActionInformation);//右击

    connect(this, &FileTreewidget::itemPressed, this, &FileTreewidget::itemPressedSlot);// 鼠标右键单击文件或文件夹时所执行的操作
    connect(this, &FileTreewidget::itemDoubleClicked, this, &FileTreewidget::DoubleClickpath);// 双击槽函数检测是文件还是目录
    connect(this, &FileTreewidget::collapseItemSignal, this, &FileTreewidget::collapseItem);//折叠
}

FileTreewidget::~FileTreewidget()
{
    delete ui;
}

// 创建文件树的根
void FileTreewidget::CreateTopItem(QString* path)
{
    QDir *rootdir = new QDir(*path);
    root = new QTreeWidgetItem();
    this->addTopLevelItem(root);
    root->setText(0,rootdir->dirName());
    FindFile(rootdir,root);
    QApplication::processEvents(); //防止界面假死
    root->setExpanded(true);
}

void FileTreewidget::setBuildwidget()
{
    buildFileWidget = new BuildWidget();
    buildDirWidget = new BuildWidget();
    reNameWidget = new BuildWidget();

    fileEdit = buildFileWidget->getLineEdit();
    dirEdit = buildDirWidget->getLineEdit();
    reNameEdit = reNameWidget->getLineEdit();

    buildFileWidget->hide();
    buildDirWidget->hide();
    reNameWidget->hide();

    buildFileWidget->setLabelText("新建文件");
    buildDirWidget->setLabelText("新建文件夹");
    reNameWidget->setLabelText("重命名");

    connect(this, &FileTreewidget::buildFile, this, [this](){
        buildFileWidget->show();
    });

    connect(this, &FileTreewidget::buildFolder, this, [this](){
        buildDirWidget->show();
    });

    connect(this, &FileTreewidget::reNameSignals, this, [this](){
        reNameWidget->show();
    });

    connect(fileEdit, &QLineEdit::editingFinished, this, [this](){
        if (fileEdit->isModified())
        {
            bulidNewFileSlot();
            fileEdit->clear();
        }
        buildFileWidget->close();
    });

    connect(dirEdit, &QLineEdit::editingFinished, this, [this](){
        if (dirEdit->isModified())
        {
            buildNewDirSlot();
            dirEdit->clear();
        }
        buildDirWidget->close();
    });

    connect(reNameEdit, &QLineEdit::editingFinished, this, [this](){
        if (reNameEdit->isModified())
        {
            reName();//重命名
            reNameEdit->clear();
        }
        reNameWidget->close();
    });
}

QTreeWidgetItem* FileTreewidget::findTargetItem(QTreeWidgetItem* item, QString target)
{
    if (item->toolTip(0) == target) {
        return item;
    }

    for (int i = 0; i < item->childCount(); ++i) {
        QTreeWidgetItem* found = findTargetItem(item->child(i), target);
        if (found) {
            return found;
        }
    }

    return nullptr;
}

// 创建文件树
void FileTreewidget::FindFile(QDir *path,QTreeWidgetItem *parent)//递归创建文件树
{
    QApplication::processEvents(); //防止界面假死
    QStringList AllDirName = path->entryList();  //返回所有文件名
    QString PathContent = path->absolutePath();
    for(int i=2;i<AllDirName.size();i++)//因为0和1是特殊目录
    {
        QFileInfo *FileInfo = new QFileInfo(PathContent+"/"+AllDirName[i]);//绝对路径和文件名
        if(FileInfo->isFile())//判断是否为文件
        {
            QTreeWidgetItem *child = new QTreeWidgetItem(parent); //创建对象并添加所属关系
            child->setIcon(0,QIcon(":/wenjian.png")); //设置Item的图标，也可以通过QSS设置。
            child->setText(0,AllDirName[i]); //设置Item的名字及所扫描到的文件名。
            child->setToolTip(0,PathContent+"/"+AllDirName[i]);
        }
        else if(FileInfo->isDir())
        {
            QDir *NextDir = new QDir(PathContent+"/"+AllDirName[i]); //返回包含文件名的绝对路径。
            QTreeWidgetItem *child = new QTreeWidgetItem(parent);  //创建对象并添加所属关系。
            child->setIcon(0,QIcon(":/wenjian-.png")); //设置Item的图标，也可以通过QSS设置。
            child->setText(0,AllDirName[i]);  //设置Item的名字及所扫描到的文件名。
            child->setToolTip(0,PathContent+"/"+AllDirName[i]); //鼠标移动到Item上所显示的内容，这里为绝对路径。
            FindFile(NextDir,child);
        }
    }
}

// 双击槽函数检测是文件还是目录
void FileTreewidget::DoubleClickpath(QTreeWidgetItem *item, int /*column*/){
    QString absolute_Path = item->toolTip(0);
    QFileInfo *FileInfo = new QFileInfo(absolute_Path);
    this->resizeColumnToContents(0);

    if(FileInfo->isFile())
    {
        // 发送信号量打开文件
        emit openFileSignals(nowItem->toolTip(0));
        emit nowFilePath(nowItem->toolTip(0));
    }
}

// 鼠标右键单击文件或文件夹时所执行的操作
void FileTreewidget::itemPressedSlot(QTreeWidgetItem * pressedItem, int column){
    this->resizeColumnToContents(0);
    nowItem = pressedItem;
    nowCol = column;
    if(qApp->mouseButtons() == Qt::RightButton)   //判断是否为右键
    {
        QString absolute_Path = pressedItem->toolTip(0);
        QFileInfo FileInfo(absolute_Path);
        if(FileInfo.isFile()){
            fileMenu->exec(QCursor::pos()); //显示右键菜单栏
        }
        else if (FileInfo.isDir()){
            dirMenu->exec(QCursor::pos());  //显示右键菜单栏
        }
        else {
            rootMenu->exec(QCursor::pos()); //点击root
        }

    }
}

void FileTreewidget::reName()
{
    QTreeWidgetItem* parent = nowItem->parent();
    QString name = reNameEdit->text();
    QFileInfo info = QFileInfo(nowItem->toolTip(0));

    QString abslute = info.absolutePath() +  QDir::separator();
    QString newPath = abslute + name;

    if (QFileInfo::exists(newPath) && QFileInfo(newPath).isFile()) {
        QMessageBox::warning(this, "错误", "文件名已存在！");
        return;
    }
    QFile::rename(info.absoluteFilePath(), newPath);

    parent->removeChild(nowItem);
    QTreeWidgetItem* child = new QTreeWidgetItem(parent);
    child->setText(0,name);
    child->setIcon(0,QIcon(":/wenjian.png"));
    child->setToolTip(0,newPath);

    nowItem->setToolTip(0, newPath);
}

// 右击菜单栏执行操作的具体方法n
void FileTreewidget::tempActionInformation(QAction *act){

    qDebug() << "Item " <<act->text();

    if (act->text() == "新建文件"){//
        emit buildFile();
    }

    if (act->text() == "保存文件")
    {
        emit saveFile(nowItem->toolTip(0));
    }

    if (act->text() == "添加文件"){//
        addFile();
    }

    if (act->text()=="新建文件夹      "){//
        emit buildFolder();
    }

    if (act->text() == "删除文件夹"){//
        deleteDir();
    }

    if (act->text() == "移除文件夹"){//
        removeDir();
    }

    if(act->text() == "移除文件"){//
        removeFile();
    }

    if(act->text() == "关闭项目"){//
        closeProject();
    }

    if (act->text() == "打开文件")
    {
        emit openFileSignals(nowItem->toolTip(0));
    }

    if (act->text() == "删除文件")//
    {
        deleteFile();
    }

    if(act->text() == "重命名"){
        reNameSignals();
    }

    if (act->text() == "打开所在文件夹"){
        QString path = nowItem->toolTip(0);
        QFileInfo info = QFileInfo(path);
        QString dir_path = info.absolutePath();
        QDesktopServices::openUrl(QUrl::fromLocalFile(dir_path));
    }

}

// 文件夹折叠的实现主要为顶部按钮提供方法，顶部按钮的实现下次再讲
void FileTreewidget::buttonCollapseItem(){
    QTreeWidgetItem *t = this->currentItem();//获取当前的
    if(t==0)
    {
        delete t;
        return;
    }

    if(t->childCount()==0){
        QTreeWidgetItem *p = t->parent();
        if(p==0){
            return;
        }
        else{
            emit collapseItemSignal(p); //折叠文件夹
        }
    }
    else{
        emit collapseItemSignal(t);
    }
}

bool FileTreewidget::setRootPath(QString* path)
{

    QRegularExpression  regex("^(([a-zA-Z]:|\\\\)\\\\)?(((\\.)|(\\.\\.)|([^\\\\/:*?\"<>|\\r\\n]+\\\\?))+$)");
    if (!regex.match(*path).hasMatch())
    {
        qDebug() << "地址错误！ " << Qt::endl;
            return false;
    }

    CreateTopItem(path);

    return true;
}

void FileTreewidget::addFile()
{
    QTreeWidgetItem* item = this->currentItem();

    QString caption = "添加文件";//对话框标题
        QString parentPath = item->toolTip(0);//打开文件的默认目录
    QString filter = "*.txt;*.asm;*.bin;*.exe;*.com";

    QString fileName = QFileDialog::getOpenFileName(
        this,
        caption,
        parentPath,
        filter
        );

    if (fileName.isEmpty())
    {
        QMessageBox::warning(this, "Warning!", "Failed to open the video!");
    }
    else
    {
        QFileInfo* addFile = new QFileInfo(fileName);
        QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
        child->setText(0,addFile->fileName());
        child->setIcon(0,QIcon(":/wenjian.png"));
        child->setToolTip(0,fileName);
    }
    delete item;
    qDebug() << parentPath;
}

void FileTreewidget::closeProject()
{
    QTreeWidgetItem *item = this->currentItem();
    QTreeWidgetItem* parItem=item->parent();
    if(parItem == 0)
    {
        delete item;
    }
    else {
        parItem->removeChild(item) ;
        delete item;
    }

    emit closeFileTree();
}

void FileTreewidget::deleteFile()//删除文件
{
    QString filePath = this->currentItem()->toolTip(0);
    QFile* defile = new QFile(filePath);

    if (defile->exists()){
        if (QFile::remove(filePath))
        {
            closeProject();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("文件删除失败！"));
        }
    }
    else{
        QMessageBox::warning(this, tr("警告"), tr("文件不存在！"));
    }

}

void FileTreewidget::deleteDir()//删除文件夹
{
    QDir dir(this->currentItem()->toolTip(0));
    if (dir.exists())
    {
        if (dir.removeRecursively())
        {
            closeProject();

        }
        else{
            QMessageBox::warning(this, tr("错误"), tr("文件夹删除失败！"));
        }
    }
    else{
        QMessageBox::warning(this, tr("警告"), tr("文件夹不存在！"));
    }
}

void FileTreewidget::removeFile()
{
    QTreeWidgetItem *item = this->currentItem();
    QTreeWidgetItem* parItem=item->parent();
    if(parItem == 0)
    {
        delete item;
    }
    else {
        parItem->removeChild(item) ;
    }

}

void FileTreewidget::removeDir()
{
    closeProject();
}

void FileTreewidget::bulidNewFile(QString name, QString path, QString suffix)
{
    if (name == ""){
        QMessageBox::warning(this, tr("警告"),//提示窗
                                                     tr("文件名不能为空"));
        return;
    }

    name += suffix;
    path += name;

    QFile file(path);

    if (file.exists()){
        QMessageBox::warning(this, tr("警告"),
                             tr("文件已存在"));
        return;
    }

    file.open(QIODevice::ReadWrite | QIODevice::Text); //读写，视为文本文件

    file.close();
    if (nowItem != NULL)
    {
        QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
        child->setFlags(child->flags() | Qt::ItemIsEditable);
        child->setText(0,name);
        child->setIcon(0,QIcon(":/wenjian.png"));
        child->setToolTip(0,path);
    }
}


void FileTreewidget::m_mkdir(QString path, QString name)//给定路径和名称，创建文件夹
{
    QDir dir(path);
    QString fullPath = path + "/" + name;
    if (dir.exists(fullPath))
    {
        if (QMessageBox::warning(
                this, "警告", "文件项目已存在，是否情况并覆盖？" ,
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QDir(fullPath).removeRecursively();
        }
        else
        {
            return;
        }
    }
    dir.mkdir(name);
}

void FileTreewidget::m_mkfile(QString path)
{
    QFile file(path);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            // 文件创建成功，您可以在这里写入内容
            file.close();
        }
    }
    else
    {
        if (QMessageBox::warning(
                this, "警告", ".asm文件已存在，是否情况并覆盖？" ,
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QFile(path).remove();
        }
        else
        {
            return;
        }

        if (file.open(QIODevice::WriteOnly)) {
            // 文件创建成功，您可以在这里写入内容
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "错误", "文件打开失败");
            return;
        }
    }
}

void FileTreewidget::m_mkConfile(QString path, QString text)
{
    QFile file(path);
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly)) {
            // 文件创建成功，您可以在这里写入内容
            QTextStream out(&file);
            out << text;
            file.close();
        }
    }
    else
    {
        if (QMessageBox::warning(
                this, "警告", ".asm文件已存在，是否情况并覆盖？" ,
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            QFile(path).remove();
        }
        else
        {
            return;
        }

        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // 文件创建成功，您可以在这里写入内容
            QTextStream out(&file);
            out << text;
            file.close();
        }
        else
        {
            QMessageBox::critical(this, "错误", "文件打开失败");
            return;
        }
    }
}

void FileTreewidget::newProject(QString pathName)//创建新建文件的文件树
{
    QFileInfo info(pathName);

    QString proName = info.baseName();
    QString abslute_path = info.path();
    m_mkdir(abslute_path, proName);

    abslute_path += "/" + proName;
    proName = "build";
    m_mkdir(abslute_path, proName);
    proName = ".avs";
    m_mkdir(abslute_path, proName);
    QString filePath = abslute_path += "/" + proName + "/" + "AVSMakeList.txt";
    m_mkConfile(filePath, pathName);

    if (isChecked)
    {
        QString newFilePath = pathName + "/" + "/main.asm";
        m_mkfile(newFilePath);
    }

    QString p = pathName;
    CreateTopItem(&p);
    isChecked = false;
}

void FileTreewidget::createAsmCheckedSlots()
{
    isChecked = true;
}

void FileTreewidget::openFileSlot(QString pathName)
{
    CreateTopItem(&pathName);
}

// 创建文件的具体实现
void FileTreewidget::bulidNewFileSlot(){
    QString fileName = fileEdit->text();
    if (fileName==""){
        QMessageBox::warning(this, tr("警告"),//提示窗
                                                     tr("文件名不能为空"));
        return;
    }
    QString tempName;
    tempName = fileName;
    QString path = nowItem->toolTip(0)+"/"+tempName;
    QFile file(path);

    if (file.exists()){
        QMessageBox::warning(this, tr("警告"),
                             tr("文件已存在"));
        return;
    }

    file.open(QIODevice::ReadWrite | QIODevice::Text); //读写，视为文本文件

    file.close();

    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
    child->setFlags(child->flags() | Qt::ItemIsEditable);
    child->setText(0,tempName);
    child->setIcon(0,QIcon(":/wenjian.png"));
    child->setToolTip(0,path);
}

void FileTreewidget::buildNewDirSlot(){
    QString dirName = dirEdit->text();
    if(dirName=="" && fileEdit->isModified()){
        QMessageBox::warning(this, tr("Application"),
                             tr("文件名不能为空"));
        return;
    }
    QString path = nowItem->toolTip(0)+"/"+dirName;
    QDir *temp = new QDir;
    bool exist = temp->exists(path);
    if(exist){
        QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
        return;
    }
    temp->mkdir(path);
    QTreeWidgetItem *child = new QTreeWidgetItem(nowItem);
    child->setFlags(child->flags() | Qt::ItemIsEditable);
    child->setText(0,dirName);
    child->setIcon(0,QIcon(":/wenjian-.png"));
    child->setToolTip(0,path);
}



#include "newwidget.h"
#include <QApplication>
#include <QDesktopServices>
#include "ui_newwidget.h"
#include <QMouseEvent>
#include <QStorageInfo>
#include <QRegularExpression>
#include <QFileDialog>
#include <QMessageBox>
#include "settings.h"

Newwidget::Newwidget(QWidget *parent)
    : FramelessWidget(parent, Func::Close)
    , selectRow(0)//默认为0
    , suffix(".bin")
    , pathIsErr(false)
    , proIsErr(false)
    , lineEditPath(new MyLineEdit(this))
    , lineEditPro(new MyLineEdit(this))
    , ui(new Ui::newwidget)
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();
    QLabel* menu_label = FramelessWidget::getmenu_label();
    ui->setupUi(main_widget);
    main_widget->layout()->addWidget(ui->main_widget);

    setWidgetFixed(602, 392);

    localDrives = getLocalDrives();
    QString drivePattern = "(" + localDrives->join("|") + "):";
    pathReg = QRegularExpression("^" + drivePattern + R"([\\/][^\\/:*?"<>|]+([\\/][^\\/:*?\"<>|]+)*$)");

    proReg = QRegularExpression("^[\\w]+$");

    editWidgetStyle = R"(
        QWidget:hover {
            border: 2px solid rgb(53, 116, 240);
    }
        QWidget{
            border: 1px solid rgb(81, 82, 82);
    }
)";

    errEditStyle = R"(
        QWidget{
            border: 2px solid rgb(222, 79, 79);
    }
)";

    menu_label->setText(" 新建文件");
    setButton();

    ui->project_widget->layout()->addWidget(lineEditPro);
    ui->widget_3->layout()->addWidget(lineEditPath);

    ui->project_widget->setStyleSheet("border:none");
    ui->widget_3->setStyleSheet("border:none");
    lineEditPro->setStyleSheet("border:none");
    lineEditPath->setStyleSheet("border:none");

    ui->project_widget->setStyleSheet(editWidgetStyle);
    ui->path_widget->setStyleSheet(editWidgetStyle);
    this->setMouseTracking(true);


    connect(lineEditPath, &QLineEdit::textChanged, this, [this](){
        if (lineEditPath->text() == "")
        {
            ui->path_widget->setStyleSheet("border: 2px solid rgb(53, 116, 240);");
            pathIsErr = false;
            return;
        }

        if (!pathReg.match(lineEditPath->text()).hasMatch())//正则判断
        {
            ui->path_widget->setStyleSheet(errEditStyle);
            pathIsErr = true;
        }
        else
        {
            ui->path_widget->setStyleSheet("border: 2px solid rgb(53, 116, 240)");
            pathIsErr = false;
        }
    });

    connect(lineEditPro, &QLineEdit::textChanged, this, [this](){
        if (lineEditPro->text() == "")
        {
            ui->project_widget->setStyleSheet("border: 2px solid rgb(53, 116, 240)");
            proIsErr = false;
            return;
        }

        if (!proReg.match(lineEditPro->text()).hasMatch())//正则判断
        {
            ui->project_widget->setStyleSheet(errEditStyle);
            proIsErr = true;
        }
        else
        {
            ui->project_widget->setStyleSheet("border: 2px solid rgb(53, 116, 240)");
            proIsErr = false;
        }
    });

    connect(ui->cancel_button, &QToolButton::clicked, this, [this](){
        lineEditPro->clear();
        lineEditPath->clear();
        this->close();
    });

    connect(ui->select_button, &QToolButton::clicked, this, [this](){

        if (lineEditPath->text() == "" || lineEditPro->text() == "")
        {
            QMessageBox::critical(this, "错误", "路径或名称为空，请输入。\n");
            return;
        }

        QFileInfo info = QFileInfo(lineEditPath->text());
        if (!info.isDir())
        {
            QMessageBox::critical(this, "错误", "所写路径不是文件夹，请输重新输入。\n");
            return;
        }

        if (!proIsErr && !pathIsErr)
        {
            selectRow = ui->tableWidget->currentRow();//获取当前点击的行号
            // switch(selectRow)
            // {
            // case 0:
            //     suffix = ".bin";
            //     break;
            // case 1:
            //     suffix = ".com";
            //     break;
            // case 2:
            //     suffix = ".exe";
            //     break;
            // default:
            //     break;
            // }
            if (ui->checkBox->isChecked())
            {
                emit createAsmChecked();
            }

            if (ui->checkBox_2->isChecked())
            {
                SETTINGS_UI().setValue(_NEW_DEFAULT_PATH, lineEditPath->text());
            }

            QString path = (lineEditPath->text() + "/" + lineEditPro->text());
            emit PathEditContent(path);
            lineEditPro->clear();
            lineEditPath->clear();
            this->close();
        }
        else
        {
            QMessageBox::critical(this, "错误", "路径或名称输入有误，请重试。\n");
        }
    });

    connect(ui->pathbutton, &QToolButton::clicked, this, [this](){
        QString caption = "选择文件";//对话框标题
        QString parentPath = "";

        QString dirName = QFileDialog::getExistingDirectory(
            this,
            caption,
            parentPath
            );

        if (!dirName.isEmpty())
        {
            lineEditPath->setText(dirName);//填入项目地址
        }
    });

    connect(lineEditPro, &MyLineEdit::textChanged, this, [this](const QString& s) {
        QString path = QString("%1/%2").arg(lineEditPath->text(), s);
        path = QDir::fromNativeSeparators(path).normalized(QString::NormalizationForm_C);
        ui->addressPrevLabel->setText(QString("项目将创建于: %1").arg(path));
    });
    connect(lineEditPath, &MyLineEdit::textChanged, this, [this](const QString& s) {
        QString path = QString("%1/%2").arg(s, lineEditPro->text());
        path = QDir::fromNativeSeparators(path).normalized(QString::NormalizationForm_C);
        ui->addressPrevLabel->setText(QString("项目将创建于: %1").arg(path));
    });
}

Newwidget::~Newwidget()
{
    delete ui;
}

MyLineEdit* Newwidget::getEditPath()
{
    return lineEditPath;
}

MyLineEdit* Newwidget::getEditPro()
{
    return lineEditPro;
}

void Newwidget::editClear()
{
    // lineEditPath->clear();
    lineEditPath->setText(SETTINGS_UI().value(_NEW_DEFAULT_PATH).toString());
    lineEditPro->clear();
    proIsErr = false;
    pathIsErr = false;
    ui->project_widget->setStyleSheet(editWidgetStyle);
    ui->path_widget->setStyleSheet(editWidgetStyle);
}

QTableWidget* Newwidget::getTableWidget()
{
    return ui->tableWidget;
}

void Newwidget::pressedStyle(QToolButton* button)
{
    QString style = R"(
        QToolButton{
            background-color:rgb(29, 84, 92);
        }
    )";
    button->setStyleSheet(style);
}

void Newwidget::resetStyleButton(QWidget* widget)
{
    QList<QToolButton*> toolButtons = widget->findChildren<QToolButton*>();

    QString style = R"(
        QToolButton{
            border:none;
        QWidget{
            border: 1px solid rgb(81, 82, 82);
        }
}
)";

    for (auto t  : toolButtons)
    {
        t->setStyleSheet(style);
    }

    toolButtons.clear();
}

void Newwidget::setButton()
{
    connect(lineEditPro, &MyLineEdit::clicked, this, [this](){
        ui->path_widget->setStyleSheet(editWidgetStyle);

        if (!proIsErr)
        {
            ui->project_widget->setStyleSheet("border: 2px solid rgb(53, 116, 240)");
        }
        else
        {
            ui->project_widget->setStyleSheet(errEditStyle);
        }
    });

    connect(lineEditPath, &MyLineEdit::clicked, this, [this](){
        ui->project_widget->setStyleSheet(editWidgetStyle);

        if (!pathIsErr)
        {
            ui->path_widget->setStyleSheet("border: 2px solid rgb(53, 116, 240)");
        }
        else
        {
            ui->path_widget->setStyleSheet(errEditStyle);
        }
    });


    ui->pathbutton->setIcon(QIcon(":/file.png"));
}

//QString Newwidget::selectFile()
//{

//}

QStringList* Newwidget::getLocalDrives()//获取本地磁盘盘符用于正则表达式
{
    QStringList* drivesList = new QStringList();
    QList<QStorageInfo> drives = QStorageInfo::mountedVolumes();
    for (const auto &drive : drives)
    {
        if (drive.isValid() && drive.isReady())
            drivesList->append(drive.rootPath().at(0));
    }
    return drivesList;
}

void Newwidget::mousePressEvent(QMouseEvent* event)
{
    QRect rect1 = ui->project_widget->rect();
    QRect rect2 = ui->path_widget->rect();
    QPoint globalPos = QCursor::pos();

    QPoint proPoint = ui->project_widget->mapFromGlobal(globalPos);
    QPoint pathPoint = ui->path_widget->mapFromGlobal(globalPos);

//    if (!rect1.contains(proPoint) && !isErr)
//    {
////        ui->project_widget->setStyleSheet(editWidgetStyle);
//    }
//    if (!rect2.contains(pathPoint) && !isErr)
//    {
////        ui->path_widget->setStyleSheet(editWidgetStyle);
//    }

    event->accept();

    FramelessWidget::mousePressEvent(event);
}

void Newwidget::mouseMoveEvent(QMouseEvent* event)
{
    FramelessWidget::mouseMoveEvent(event);
    event->accept();
}



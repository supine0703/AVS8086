#include "leftwidget.h"
#include "ui_leftwidget.h"
#include <QIcon>
#include <QPixmap>
#include <QPainter>
#include <QList>

Leftwidget::Leftwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::leftwidget)
{
    ui->setupUi(this);

    initList();
    initStyle();
    initButton();
    setButton();
    setImages();

    connect(ui->run_button, &QToolButton::clicked, this, [this](){
        emit runButton_clicked();
    });
}

Leftwidget::~Leftwidget()
{
    delete ui;
}

void Leftwidget::setImages()
{
    ui->func_button->setIcon(QIcon(":/24gf-ellipsis.png"));
    ui->run_button->setIcon(QIcon(":/run.png"));
    ui->compile_button->setIcon(QIcon(":/bianyigoujian.png"));
    ui->pro_button->setIcon(QIcon(":/wenjian--.png"));
}

QToolButton* Leftwidget::getFuncButton()
{
    return ui->func_button;
}

void Leftwidget::initButton()
{
    for (auto button : ListButtons)
    {
        button->setStyleSheet(style1);
    }
}

void Leftwidget::setButton()
{
    for (auto button : ListButtons)//初始化样式
    {
        connect(button, &QToolButton::clicked, this, [button,this](){
            button->setStyleSheet(style2);
        });
    }

    connect(ui->pro_button, &QToolButton::clicked, this, [this](){
        emit proButton_clicked();
    });

    connect(ui->func_button, &QToolButton::clicked, this, [this](){
        emit funcButton_clicked();
    });
}

void Leftwidget::initStyle()
{
    style1 = R"(
        QToolButton:hover{
            background-color:rgb(77, 80, 86);
    }
        QToolButton{
        border:none;
        border-radius:7px;
        color:rgb(239, 239, 239);
    }
)";

    style2 = R"(
        QToolButton{
        border:none;
        background-color:rgb(53, 116, 240);
        border-radius:7px;
        color:rgb(239, 239, 239);
    }
)";
}

void Leftwidget::initList()
{
    ListButtons = ui->left_widget->findChildren<QToolButton*>();

}

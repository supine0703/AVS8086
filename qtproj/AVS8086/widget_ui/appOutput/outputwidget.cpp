#include "outputwidget.h"
#include "ui_outputwidget.h"

outputwidget::outputwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::outputwidget)
{
    ui->setupUi(this);
}

outputwidget::~outputwidget()
{
    delete ui;
}

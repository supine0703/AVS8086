#include <QMouseEvent>
#include "buildwidget.h"
#include "ui_buildwidget.h"

BuildWidget::BuildWidget(QWidget *parent)
    : FramelessWidget("buile", parent, Func::Close)
    , isLeftPress(false)
    , ui(new Ui::BuildWidget)
{
    QWidget* main_widget = FramelessWidget::getMain_Widget();
    ui->setupUi(main_widget);
    main_widget->layout()->addWidget(ui->widget);

    setWidgetFixed(350, 110);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
}

BuildWidget::~BuildWidget()
{
    delete ui;
}

void BuildWidget::setLabelText(QString text)
{
    ui->label->setText(text);
}

QLineEdit*BuildWidget::getLineEdit()
{
    return ui->lineEdit;
}


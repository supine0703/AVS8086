#include "widget.h"

Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    this->setWindowTitle(
        QString("AVS8086 v%1 - assembler virtual simulator for 8086")
            .arg(THE_VESION)
    );
}

Widget::~Widget()
{
}

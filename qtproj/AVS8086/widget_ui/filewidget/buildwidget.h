#ifndef BUILDWIDGET_H
#define BUILDWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "FramelessWidget.h"

namespace Ui {
class BuildWidget;
}

class BuildWidget : public FramelessWidget
{
    Q_OBJECT

public:
    explicit BuildWidget(QWidget *parent = nullptr);
    ~BuildWidget();

    void setLabelText(QString text);
    QLineEdit* getLineEdit();



private:
    Ui::BuildWidget *ui;

    bool isLeftPress;
    QPoint m_dragPosition;

};

#endif // BUILDWIDGET_H

#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>

namespace Ui {
class outputwidget;
}

class outputwidget : public QWidget
{
    Q_OBJECT

public:
    explicit outputwidget(QWidget *parent = nullptr);
    ~outputwidget();

private:
    Ui::outputwidget *ui;
};

#endif // OUTPUTWIDGET_H

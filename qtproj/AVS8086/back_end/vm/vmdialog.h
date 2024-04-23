#ifndef VMDIALOG_H
#define VMDIALOG_H

#include <QDialog>

namespace Ui {
class VMDialog;
}

class VMDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VMDialog(QWidget* parent = nullptr);
    ~VMDialog();

private:
    Ui::VMDialog* ui;
};

#endif // VMDIALOG_H

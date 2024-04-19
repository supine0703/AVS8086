#include "vmdialog.h"
#include "ui_vmdialog.h"

VMDialog::VMDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::VMDialog)
{
    ui->setupUi(this);
}

VMDialog::~VMDialog()
{
    delete ui;
}

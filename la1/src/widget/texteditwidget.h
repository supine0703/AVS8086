#ifndef TEXTEDITWIDGET_H
#define TEXTEDITWIDGET_H

#include <QWidget>

namespace Ui {
class TextEditWidget;
}

class QStatusBar;

class TextEditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextEditWidget(QWidget* parent = nullptr);
    ~TextEditWidget();

private slots:
    void on_btn_choose_clicked();

    void on_btn_open_clicked();

    void on_btn_save_clicked();

    void on_btn_compile_clicked();

    void on_textEdit_cursorPositionChanged();

private:
    Ui::TextEditWidget* ui;
    QStatusBar* status;
};

#endif // TEXTEDITWIDGET_H

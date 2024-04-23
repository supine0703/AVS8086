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
    void on_pushButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_textEdit_cursorPositionChanged();

private:
    Ui::TextEditWidget* ui;
    QStatusBar* status;
};

#endif // TEXTEDITWIDGET_H

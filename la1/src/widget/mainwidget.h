#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QWidget>

namespace Ui {
class MainWidget;
}

class QStatusBar;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private slots:
    void on_btn_choose_clicked();

    void on_btn_open_clicked();

    void on_btn_save_clicked();

    void on_btn_compile_clicked();

    void on_textEdit_cursorPositionChanged();

    void on_textEdit_textChanged();

    void on_textEdit_blockCountChanged(int blockCount);

private:
    void useCodeEdit();

    Ui::MainWidget* ui;
    QStatusBar* status;
};

#endif // MAIN_WIDGET_H

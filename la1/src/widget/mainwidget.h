/**
 *  AVS8086 - See <https://github.com/supine0703/AVS8086> to know more.
 *
 *  Copyright (C) <2024>  <李宗霖>  github <https://github.com/supine0703>.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


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
    void initCodeEdit();

    Ui::MainWidget* ui;
    QStatusBar* status;

    int m_charWidth;
    int m_charHeight;
};

#endif // MAIN_WIDGET_H

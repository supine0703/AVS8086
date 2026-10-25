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

#include <QTextCharFormat>
#include <QWidget>

namespace Ui {
class MainWidget;
}

class CodeEdit;
class SearchEdit;
class QStatusBar;
class QPushButton;
class QTabWidget;
class QTextBrowser;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget* parent = nullptr);
    ~MainWidget();

private slots:
    void on_edit_path_textChanged(const QString& path);
    void on_btn_choose_path_clicked();
    void on_btn_clear_path_clicked();
    void on_btn_open_file_clicked();
    void on_btn_clear_file_clicked();
    void on_btn_save_file_clicked();
    void on_btn_save_file_as_clicked();
    void on_edit_code_cursorPositionChanged();
    void on_edit_code_blockCountChanged(int blockCount);
    void on_edit_code_textChanged();
    void syncLineNumToEdit(int lineV);
    void editEndOfLineChanged(const QString& text);
    void onAnchorClicked(const QUrl& link);

    void on_btn_lexer_clicked();
    void on_btn_parser_clicked();
    void on_btn_to_bin_clicked();

private:
    void initResultTab();
    void initCodeEdit();
    void initLineSearch();
    void initStatusBar();
    void loadCodeLineColor();
    void setFocusLine(int lineNum);
    void setCodeLineWidth(int bitNum);
    void saveEditTextTo(const QString& path);
    void connectCodeLineChangedWithEdit();
    void disconnectCodeLineChangedWithEdit();

private:
    Ui::MainWidget* ui;

    QStatusBar* m_status;
    SearchEdit* m_search;
    CodeEdit* m_edit;
    QPushButton* m_btn_end_of_line;
    QTabWidget* m_result_tab;

    QTextBrowser* m_result_1;
    QTextBrowser* m_result_2;
    QTextBrowser* m_result_3;

    QTextCharFormat m_color_f_1;
    QTextCharFormat m_color_f_2;
    int m_focus_line;
    int m_char_width;

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
};

#endif // MAIN_WIDGET_H

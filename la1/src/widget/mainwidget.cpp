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


#include "mainwidget.h"

#include "../settings/settings.h"
#include "codeedit.h"
#include "ui_mainwidget.h"

#include <QFileDialog>
#include <QFontMetrics>
#include <QMessageBox>
#include <QRegularExpression>
#include <QScrollBar>
#include <QStatusBar>
#include <QTextBlock>

#define CODE_LINE_SPACE_WIDTH     2
#define CODE_LINE_DEFAULT_COLOR_1 QColor(165, 165, 165)
#define CODE_LINE_DEFAULT_COLOR_2 QColor(Qt::red)//QColor(15, 15, 15)

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWidget), status(new QStatusBar(this)), edit(nullptr)
{
    // 设置窗口标题
    this->setWindowTitle(
        QString("AVS8086 v%1 - assembler virtual simulator for 8086").arg(THE_VERSION)
    );

    // 计算字符宽度 (默认使用等宽字符，非等宽字符可能会造成不可预知的问题)
    auto fm = QFontMetrics(QApplication::font());
    m_charWidth = fm.horizontalAdvance('0');

    // 初始化 UI 以及各个组件
    ui->setupUi(this);
    initCodeEdit();
    ui->layout_widget->addWidget(status);
    // status->showMessage(QString("[ row: 1, col: 1 ]"));
    on_textEdit_cursorPositionChanged();

    // 读取配置文件中的文件路径，并试图打开
    if (QFile f(SETTINGS().value(_APP_OPEN_FILE_).toString()); f.exists())
    { // 文件存在
        ui->lineEdit_file->setText(f.fileName());
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui->textEdit->setPlainText(f.readAll());
            f.close();
        }
        else
        {
            QMessageBox::warning(this, "警告", "文件打开失败!\n" + f.errorString());
        }
    }
    else
    { // 文件不存在
        SETTINGS().setValue(_APP_OPEN_FILE_, "");
    }
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initCodeEdit()
{
    // code line
    ui->codeLine->document()->setDefaultTextOption(QTextOption(Qt::AlignRight)); // 右对齐
    ui->codeLine->viewport()->setCursor(Qt::ArrowCursor);                        // 箭头光标
    ui->codeLine->appendPlainText("1"); // 默认值，不能用 set
    // setFocusLine(0);                    // 设置第一行为焦点
    setCodeLineWidth(1);                // 初始化 codeLine 的宽度

    // { // 设置初始颜色
    //     // 从配置文件中读取颜色
    //     if (m_clColor_1 = QColor(SETTINGS().value(_APP_CLC_1_).toString()); !m_clColor_1.isValid())
    //     {
    //         m_clColor_1 = CODE_LINE_DEFAULT_COLOR_1;
    //         SETTINGS().setValue(_APP_CLC_1_, m_clColor_1.name());
    //     }
    //     if (m_clColor_2 = QColor(SETTINGS().value(_APP_CLC_2_).toString()); !m_clColor_2.isValid())
    //     {
    //         m_clColor_2 = CODE_LINE_DEFAULT_COLOR_2;
    //         SETTINGS().setValue(_APP_CLC_2_, m_clColor_2.name());
    //     }
    //     // 设置行号的初始颜色
    //     QTextCursor cursor(ui->codeLine->document());
    //     QTextCharFormat format;
    //     format.setForeground(m_clColor_1);
    //     cursor.select(QTextCursor::Document);
    //     cursor.setCharFormat(format);
    // }

    // code edit
    // 改用 CodeEdit 并重新绑定信号槽
    ui->layout_code->removeWidget(ui->textEdit);
    ui->textEdit->disconnect();
    ui->textEdit->deleteLater();
    ui->textEdit = edit = new CodeEdit;
    ui->layout_code->addWidget(edit);
    edit->appendPlainText("");
    edit->setEditTabSize(m_charWidth, 4);
    connect(edit, &CodeEdit::textChanged, this, &MainWidget::on_textEdit_textChanged);
    connect(edit, &CodeEdit::blockCountChanged, this, &MainWidget::on_textEdit_blockCountChanged);
    connect(
        edit, &CodeEdit::cursorPositionChanged, this, &MainWidget::on_textEdit_cursorPositionChanged
    );

    // 通过信号槽将行号与编辑器同步
    connect(
        edit->verticalScrollBar(),
        &QScrollBar::valueChanged,
        ui->codeLine->verticalScrollBar(),
        &QScrollBar::setValue
    );
    connect_codeLineChanged_with_edit();
}

void MainWidget::setFocusLine(int lineNum)
{
    // QTextCharFormat format1;
    // format1.setForeground(m_clColor_1);
    // QTextCharFormat format2;
    // format2.setForeground(m_clColor_2);

    // if (!m_focusLine.isNull())
    // { // 如果存在则恢复 Color1
    //     qDebug() << 2;
    //     m_focusLine.select(QTextCursor::BlockUnderCursor);
    //     m_focusLine.mergeCharFormat(format1);
    // }
    // qDebug() << 3;
    // // 设置 lineNum 的颜色为 Color2
    // m_focusLine = QTextCursor(ui->codeLine->document()->findBlockByNumber(lineNum));
    // m_focusLine.select(QTextCursor::BlockUnderCursor);
    // m_focusLine.mergeCharFormat(format2);
    // m_focusLine.clearSelection();
    // m_focusLine.mergeCharFormat(format1);
}

void MainWidget::setCodeLineWidth(int bitNum)
{ // 设置 codeLine 的宽度
    ui->codeLine->setMaximumWidth(m_charWidth * (bitNum + CODE_LINE_SPACE_WIDTH));
}

// 以下为信号槽部分
/* ========================================================================= */

void MainWidget::connect_codeLineChanged_with_edit()
{ // 连接行号与编辑器的滚动条
    connect(
        ui->codeLine->verticalScrollBar(),
        &QScrollBar::valueChanged,
        this,
        &MainWidget::sync_codeLine_to_edit
    );
}

void MainWidget::disconnect_codeLineChanged_with_edit()
{ // 断开行号与编辑器的滚动条的同步
    ui->codeLine->verticalScrollBar()->disconnect(this);
}

void MainWidget::on_btn_choose_clicked()
{
    auto fp = QFileDialog::getOpenFileName(
        this, "选择文件", QFileInfo(ui->lineEdit_file->text()).filePath()
    );
    if (!fp.isEmpty())
    { // 只判断字符串不为空，是否存在交给打开时判断
        ui->lineEdit_file->setText(fp);
        SETTINGS().setValue(_APP_OPEN_FILE_, fp);
    }
}

void MainWidget::on_btn_open_clicked()
{
    if (QMessageBox::question(this, "询问", "是否打开此文件:\n" + ui->lineEdit_file->text()) ==
        QMessageBox::Yes)
    {
        if (QFile f(ui->lineEdit_file->text()); f.exists())
        { // 文件存在
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            { // 文件打开成功
                ui->textEdit->setPlainText(f.readAll());
                f.close();
            }
            else
            { // 文件打开失败
                QMessageBox::warning(this, "警告", "文件打开失败!\n" + f.errorString());
            }
        }
        else
        { // 文件不存在
            QMessageBox::warning(this, "警告", "文件不存在!");
        }
    }
}

void MainWidget::on_btn_save_clicked()
{
    if (QMessageBox::question(this, "询问", "是否保存文件至:\n" + ui->lineEdit_file->text()) ==
        QMessageBox::Yes)
    {
        // QDir dir;
        if (QFileInfo fi(ui->lineEdit_file->text()); !QDir().exists(fi.filePath()))
        { // 文件夹不存在
            if (!QDir().mkpath(fi.filePath()))
            {
                QMessageBox::warning(this, "警告", "文件夹创建失败");
                return;
            }
        }
        // 优化文件结构 (去掉行尾多余空格、...)
        if (QFile f(ui->lineEdit_file->text()); f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            // 删除行尾空白
            auto cursor = edit->textCursor();
            cursor.beginEditBlock();
            { // 确保这一组操作的原子性
                auto block = edit->document()->firstBlock();
                while (block.isValid())
                {
                    QString text = block.text();
                    static const QRegularExpression re("\\s+$");
                    text.remove(re);

                    cursor.setPosition(block.position());
                    cursor.select(QTextCursor::BlockUnderCursor);
                    cursor.insertText(text);

                    block = block.next();
                }
            }
            cursor.endEditBlock();

            // ...

            f.close();
        }
        else
        {
            QMessageBox::warning(this, "警告", "文件打开失败!\n" + f.errorString());
        }
    }
}

void MainWidget::on_textEdit_cursorPositionChanged()
{
    // 在状态栏显示行列
    auto ec = edit->textCursor();
    status->showMessage(
        QString("[ row: %1, col: %2 ]").arg(ec.blockNumber() + 1).arg(ec.columnNumber() + 1)
    );

    // 设置焦点行突出
    // 注意：cursorPositionChanged 早于 blockCountChanged
    qDebug() << 1 << ec.blockNumber();
    setFocusLine(ec.blockNumber());
}

// TODO: ...
void MainWidget::on_textEdit_textChanged() {}

void MainWidget::on_textEdit_blockCountChanged(int blockCount)
{ // 显示行号 (增加 or 减少)
    qDebug() << 4 << edit->textCursor().blockNumber();
    auto line = ui->codeLine->blockCount();
    // if (line == blockCount)
    //     return;
    Q_ASSERT(line != blockCount);

    // 设置行号一侧的宽度
    if (auto bitNum = QString::number(blockCount).size(); QString::number(line).size() != bitNum)
    {
        setCodeLineWidth(bitNum);
    }

    disconnect_codeLineChanged_with_edit();
    { // 为避免同步滚动条的频繁操作，消耗资源，暂时屏蔽 line change with edit 的信号槽
        if (line < blockCount)
        { // 增加  <
            for (int i = line + 1; i <= blockCount; i++)
            {
                ui->codeLine->appendPlainText(QString::number(i));
            }
        }
        else
        { // 减少  >
            auto tc = ui->codeLine->textCursor();
            while (tc.blockNumber() >= blockCount)
            { // blockNumber 比 blockCount 小 1
                // tc.movePosition(QTextCursor::End); // 将光标移动到文本末尾 (应该一直保持在末尾)
                tc.select(QTextCursor::BlockUnderCursor); // 选择当前块
                tc.removeSelectedText();
            }
        }
    }
    connect_codeLineChanged_with_edit();
    sync_codeLine_to_edit(ui->codeLine->verticalScrollBar()->value());
}

void MainWidget::sync_codeLine_to_edit(int lineV)
{ // 同步行号与编辑器的滚动条
    if (int editV = edit->verticalScrollBar()->value(); editV != lineV)
    {
        edit->verticalScrollBar()->setValue(lineV);
    }
}

// 下面部分为功能测试
/* ========================================================================= */

// #include "service/json.h"
// #include "lexer/lexer.h"
// #include "parser/parser.h"
// // #include "assembler/assembler.h"
// // #include "vm/vm.h"

void MainWidget::on_btn_compile_clicked()
{
    // using namespace avs8086;
    // using namespace avs8086::token;
    // using namespace avs8086::lexer;
    // using namespace avs8086::parser;
    // // using namespace avs8086::assembler;
    // // using namespace avs8086::vm;

    // Lexer l(ui->lineEdit->text());
    // for (const auto& t : l.tokens())
    //     qDebug().noquote() << QString("[ %1, %2, %3 ] >> %4")
    //         .arg(t.row(), 2).arg(t.column(), 2).arg(t.length(), 2).arg(t.content());
    // auto lt(l.end());
    // qDebug().noquote() << QString("[ %1, %2, %3 ] >> %4")
    //     .arg(lt.row(), 2).arg(lt.column(), 2).arg(lt.length(), 2).arg(lt.content());
    // // qDebug() << "error:";
    // // for (const auto& e : l.infos())
    // //     qDebug() << e.typeName()
    // //              << e.position().row()
    // //              << e.position().column()
    // //              << e.position().length()
    // //              << e.value();
    // // qDebug().noquote() << l.restore(l.tokens());

    // Parser p(&l);
    // auto root = p.newAST();
    // for (const auto& e : p.infos())
    //     qDebug() << e.typeName() << ": ("
    //              << e.pos().row() << ","
    //              << e.pos().column() << ","
    //              << e.pos().length() << ") >"
    //              << e.value();

    // // Assembler a(root);
    // // a.compile();
    // // // for (const auto& e : a.errorInfos())
    // // //     qDebug() << e;

    // // // a.saveToFile();
    // // a.copy(R"(D:\Leisure\Desktop\8086\AVS\mycode.bin)",
    // //        R"(D:\Leisure\Desktop\8086\AVS\test.bin)");
    // // VM vm;
    // // vm.m_regs = a.wellInitInfos();
    // // vm.readFromFile(R"(D:\Leisure\Desktop\8086\AVS\test.bin)");


    // // for (const auto& s : root->traversal())
    // //     qDebug() << s;
    // // qDebug() << "error:";
    // // for (const auto& e : p.errorInfos())
    // //     qDebug() << e;
    // // qDebug() << "warning:";
    // // for (const auto& w : p.warningInfos())
    // //     qDebug() << w;

    // Json js;
    // js.setObject(root->json());
    // js.saveToFile(DOCS_PATH"/ast.json");
    // // qDebug() << js.getObject();

    // // qDebug() << a.wellInitInfos();
}

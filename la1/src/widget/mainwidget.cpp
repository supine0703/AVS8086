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
#include <QMessageBox>
#include <QScrollBar>
#include <QStatusBar>
#include <QFontMetrics>

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWidget), status(new QStatusBar(this))
{
    auto fm = QFontMetrics(QApplication::font());
    m_charWidth = fm.horizontalAdvance('0');
    // m_charHeight = fm.height()

    ui->setupUi(this);
    initCodeEdit();
    ui->lineEdit_file->setText(SETTINGS().value(_TEXT_EDIT_FILE_).toString());

    this->setWindowTitle(
        QString("AVS8086 v%1 - assembler virtual simulator for 8086").arg(THE_VESION)
    );
    QFile f(ui->lineEdit_file->text());
    if (f.exists())
    {
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
    ui->layout_widget->addWidget(status);
    status->showMessage(QString("row:    1  col:   1"));
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
    ui->codeLine->appendPlainText("1");                                          // 默认值
    ui->codeLine->setMaximumWidth(m_charWidth * 3);
    ui->textEdit->setStyleSheet(QString("QPlainTextEdit { line-height: 48px; }"));

    // code edit
    // 改用 code edit 并重新绑定信号槽
    ui->layout_code->removeWidget(ui->textEdit);
    ui->textEdit->disconnect();
    ui->textEdit->deleteLater();
    ui->textEdit = new CodeEdit;
    ui->layout_code->addWidget(ui->textEdit);
    connect(ui->textEdit, &CodeEdit::textChanged, this, &MainWidget::on_textEdit_textChanged);
    connect(
        ui->textEdit, &CodeEdit::blockCountChanged, this, &MainWidget::on_textEdit_blockCountChanged
    );
    connect(
        ui->textEdit,
        &CodeEdit::cursorPositionChanged,
        this,
        &MainWidget::on_textEdit_cursorPositionChanged
    );


    // connect(
    //     ui->textEdit->verticalScrollBar(),
    //     &QScrollBar::valueChanged,
    //     ui->codeLine->verticalScrollBar(),
    //     &QScrollBar::setValue
    // );
    // connect(
    //     ui->codeLine->verticalScrollBar(),
    //     &QScrollBar::valueChanged,
    //     ui->textEdit->verticalScrollBar(),
    //     &QScrollBar::setValue
    // );
}


void MainWidget::on_btn_choose_clicked()
{
    QFileInfo fi(ui->lineEdit_file->text());
    auto fp = QFileDialog::getOpenFileName(this, "选择文件", fi.filePath());
    if (!fp.isEmpty())
    {
        ui->lineEdit_file->setText(fp);
        SETTINGS().setValue(_TEXT_EDIT_FILE_, fp);
    }
}


void MainWidget::on_btn_open_clicked()
{
    if (QMessageBox::question(this, "询问", "是否打开此文件:\n" + ui->lineEdit_file->text()) ==
        QMessageBox::Yes)
    {
        QFile f(ui->lineEdit_file->text());
        if (f.exists())
        {
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
        {
            QMessageBox::warning(this, "警告", "文件不存在!");
        }
    }
}


void MainWidget::on_btn_save_clicked()
{
    if (QMessageBox::question(this, "询问", "是否保存文件至:\n" + ui->lineEdit_file->text()) ==
        QMessageBox::Yes)
    {
        QFileInfo fi(ui->lineEdit_file->text());
        QDir dir;
        if (!dir.exists(fi.filePath()))
        {
            if (!dir.mkpath(fi.filePath()))
            {
                QMessageBox::warning(this, "警告", "文件夹创建失败");
                return;
            }
        }
        QFile f(ui->lineEdit_file->text());
        if (f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (!ui->textEdit->toPlainText().isEmpty())
            {
                QString txt;
                for (const auto& t : ui->textEdit->toPlainText().split("\n"))
                {
                    txt.append(t.trimmed() + "\n"); // 去掉前后空白补上回车
                }
                if (txt.at(txt.length() - 2) == '\n')
                {
                    txt.removeLast();
                }
                ui->textEdit->setPlainText(txt);
                QTextStream out(&f);
                out << txt;
            }
            f.close();
        }
        else
        {
            QMessageBox::warning(this, "警告", "文件打开失败!\n" + f.errorString());
        }
    }
}

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


void MainWidget::on_textEdit_cursorPositionChanged()
{
    auto tc = ui->textEdit->textCursor();
    status->showMessage(QString("row: %1  col: %2")
                            .arg(tc.blockNumber() + 1, 4, 10, QChar(' '))
                            .arg(tc.columnNumber() + 1, 4, 10, QChar(' ')));
}

void MainWidget::on_textEdit_textChanged()
{
}

#include <QTextBlock>

void MainWidget::on_textEdit_blockCountChanged(int blockCount)
{
    auto line = ui->codeLine->blockCount();
    // if (line == blockCount)
    //     return;
    Q_ASSERT(line != blockCount);

    if (auto width = QString::number(blockCount).size(); QString::number(line).size() != width)
    {
        ui->codeLine->setMaximumWidth(m_charWidth * (width + 2));
    }

    if (line < blockCount)
    { // 增加  <
        for (int i = line + 1; i <= blockCount; i++)
        {
            ui->codeLine->appendPlainText(QString::number(i));
        }
    }
    else
    { // 减少  >
        auto tc = ui->codeLine->textCursor(); // blockNumber 比 blockCount 小 1
        while (tc.blockNumber() >= blockCount)
        {
            // tc.movePosition(QTextCursor::End); // 将光标移动到文本末尾 (应该一直保持在末尾)
            tc.select(QTextCursor::BlockUnderCursor); // 选择当前块
            tc.removeSelectedText();
        }
    }

    // int uniformHeight = 40;
    // auto cursor = ui->textEdit->textCursor();
    // cursor.select(QTextCursor::Document); // 选择整个文档

    // QTextBlockFormat blockFormat;
    // QTextCharFormat charFormat;

    // blockFormat.setLineHeight(uniformHeight, QTextBlockFormat::FixedHeight); // 设置固定行高
    // cursor.setBlockFormat(blockFormat);

    // QTextCharFormat format = cursor.charFormat();
    // // format.setFontPointSize(uniformHeight);
    // cursor.setCharFormat(format);

    // QTextCursor cursor = ui->textEdit->textCursor();
    // cursor.select(QTextCursor::LineUnderCursor);
    // QTextBlock block = cursor.block();
    // QFontMetrics metrics(ui->textEdit->font());

    // while (block.isValid()) {
    //     cursor = QTextCursor(block);
    //     QTextCharFormat charFormat = cursor.charFormat();
    //     QFont font = charFormat.font();

    //     // 获取当前字体的 x-height 和 cap-height
    //     QFontMetrics fontMetrics(font);
    //     int currentHeight = fontMetrics.height();
    //     double scaleFactor = static_cast<double>(uniformHeight) / currentHeight;

    //     // 根据比例缩放字体大小
    //     font.setPointSizeF(font.pointSizeF() * scaleFactor);
    //     charFormat.setFont(font);
    //     cursor.setCharFormat(charFormat);

    //     block = block.next();
    // }

}

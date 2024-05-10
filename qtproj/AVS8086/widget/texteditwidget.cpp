#include "texteditwidget.h"
#include "ui_texteditwidget.h"

#include "service/json.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
// #include "assembler/assembler.h"
// #include "vm/vm.h"

#include "settings.h"
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>

TextEditWidget::TextEditWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::TextEditWidget)
    , status(new QStatusBar(this))
{
    ui->setupUi(this);
    ui->lineEdit->setText(SETTINGS().value(_TEXT_EDIT_FILE_).toString());

    this->setWindowTitle(
        QString("AVS8086 v%1 - assembler virtual simulator for 8086")
            .arg(THE_VESION)
    );
    QFile f(ui->lineEdit->text());
    if (f.exists())
    {
        if (f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui->textEdit->setPlainText(f.readAll());
            f.close();
        }
        else
            QMessageBox::warning(
                this, "警告", "文件打开失败!\n" + f.errorString()
            );
    }
    ui->verticalLayout->addWidget(status);
}

TextEditWidget::~TextEditWidget()
{
    delete ui;
}

void TextEditWidget::on_pushButton_clicked()
{
    QFileInfo fi(ui->lineEdit->text());
    auto fp = QFileDialog::getOpenFileName(this, "选择文件", fi.filePath());
    if (!fp.isEmpty())
    {
        ui->lineEdit->setText(fp);
        SETTINGS().setValue(_TEXT_EDIT_FILE_, fp);
    }
}


void TextEditWidget::on_pushButton_6_clicked()
{
    if (QMessageBox::question(
            this, "询问", "是否打开此文件:\n" + ui->lineEdit->text()
        ) == QMessageBox::Yes)
    {
        QFile f(ui->lineEdit->text());
        if (f.exists())
        {
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ui->textEdit->setPlainText(f.readAll());
                f.close();
            }
            else
                QMessageBox::warning(
                    this, "警告", "文件打开失败!\n" + f.errorString()
                );
        }
        else
            QMessageBox::warning(this, "警告", "文件不存在!");
    }
}


void TextEditWidget::on_pushButton_7_clicked()
{
    if (QMessageBox::question(
            this, "询问", "是否保存文件至:\n" + ui->lineEdit->text()
        ) == QMessageBox::Yes)
    {
        QFileInfo fi(ui->lineEdit->text());
        QDir dir;
        if (!dir.exists(fi.filePath()))
        {
            if (!dir.mkpath(fi.filePath()))
            {
                QMessageBox::warning(this, "警告", "文件夹创建失败");
                return;
            }
        }
        QFile f(ui->lineEdit->text());
        if (f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (!ui->textEdit->toPlainText().isEmpty())
            {
                QString txt;
                for (const auto& t : ui->textEdit->toPlainText().split("\n"))
                    txt.append(t.trimmed() + "\n"); // 去掉前后空白补上回车
                if (txt.at(txt.length() - 2) == '\n')
                    txt.removeLast();
                ui->textEdit->setPlainText(txt);
                QTextStream out(&f);
                out << txt;
            }
            f.close();
        }
        else
            QMessageBox::warning(
                this, "警告", "文件打开失败!\n" + f.errorString()
            );
    }
}

using namespace avs8086;
using namespace avs8086::token;
using namespace avs8086::lexer;
using namespace avs8086::parser;
// using namespace avs8086::assembler;
// using namespace avs8086::vm;
void TextEditWidget::on_pushButton_2_clicked()
{   
    Lexer l(ui->lineEdit->text());
    for (const auto& t : l.tokens())
        qDebug().noquote() << QString("[ %1, %2, %3 ] >> %4")
            .arg(t.row(), 2).arg(t.column(), 2).arg(t.length(), 2).arg(t.content());
    auto lt(l.end());
    qDebug().noquote() << QString("[ %1, %2, %3 ] >> %4")
        .arg(lt.row(), 2).arg(lt.column(), 2).arg(lt.length(), 2).arg(lt.content());
    // qDebug() << "error:";
    // for (const auto& e : l.infos())
    //     qDebug() << e.typeName()
    //              << e.position().row()
    //              << e.position().column()
    //              << e.position().length()
    //              << e.value();
    // qDebug().noquote() << l.restore(l.tokens());

    Parser p(&l);
    auto root = p.newAST();
    for (const auto& e : p.infos())
        qDebug() << e.typeName() << ": ("
                 << e.position().row() << ","
                 << e.position().column() << ","
                 << e.position().length() << ") >"
                 << e.value();

    // Assembler a(root);
    // a.compile();
    // // for (const auto& e : a.errorInfos())
    // //     qDebug() << e;

    // // a.saveToFile();
    // a.copy(R"(D:\Leisure\Desktop\8086\AVS\mycode.bin)",
    //        R"(D:\Leisure\Desktop\8086\AVS\test.bin)");
    // VM vm;
    // vm.m_regs = a.wellInitInfos();
    // vm.readFromFile(R"(D:\Leisure\Desktop\8086\AVS\test.bin)");



    // for (const auto& s : root->traversal())
    //     qDebug() << s;
    // qDebug() << "error:";
    // for (const auto& e : p.errorInfos())
    //     qDebug() << e;
    // qDebug() << "warning:";
    // for (const auto& w : p.warningInfos())
    //     qDebug() << w;

    Json js;
    js.setObject(root->json());
    js.saveToFile(DOCS_PATH"/ast.json");
    // qDebug() << js.getObject();

    // qDebug() << a.wellInitInfos();

}


void TextEditWidget::on_textEdit_cursorPositionChanged()
{
    auto pos = ui->textEdit->textCursor();
    status->showMessage(
        QString("row: %1  col %2")
            .arg(pos.blockNumber() + 1, 4, 10, QChar(' '))
            .arg(pos.columnNumber() + 1, 4, 10, QChar(' '))
    );
}


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
#include "searchedit.h"
#include "ui_mainwidget.h"

#include <QAbstractItemView>
#include <QCompleter>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFontMetrics>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QRegularExpression>
#include <QScrollBar>
#include <QStatusBar>
#include <QStringListModel>
#include <QStyledItemDelegate>
#include <QTabWidget>
#include <QTextBlock>
#include <QTextBrowser>

#define _APP_OPEN_FILE_  "/app/open_file"
#define _APP_CL_COLOR_1_ "/app/code_line_color_1"
#define _APP_CL_COLOR_2_ "/app/code_line_color_2"

#define CODE_LINE_SPACE_WIDTH     3
#define CODE_LINE_DEFAULT_COLOR_1 QColor(165, 165, 165)
#define CODE_LINE_DEFAULT_COLOR_2 QColor(225, 15, 15); // QColor(15, 15, 15)

MainWidget::MainWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
    , m_status(new QStatusBar(this))
    , m_btn_end_of_line(new QPushButton("EOL", this))
    , m_result_tab(new QTabWidget)
    , m_edit(nullptr)
    , m_focus_line(-1)
    /* 计算字符宽度 (默认使用等宽字符，非等宽字符可能会造成不可预知的问题) */
    , m_char_width(QFontMetrics(QApplication::font()).horizontalAdvance('0'))
{
    ui->setupUi(this);
    // 设置窗口标题 (得在 ui 初始化之后)
    this->setWindowTitle(QString("AVS8086 v%1 - assembler virtual simulator for 8086").arg(THE_VERSION));

    // 初始化 UI 以及各个组件
    initResultTab();
    initCodeEdit();
    initLineSearch();
    initStatusBar();
    on_edit_code_cursorPositionChanged(); // 状态栏显示行列、突出焦点

    // 读取配置文件中的文件路径，并试图打开
    if (QFile f(SETTINGS().value(_APP_OPEN_FILE_).toString()); f.exists())
    { // 文件存在
        ui->edit_path->setText(f.fileName());
        if (f.open(QIODevice::ReadOnly))
        {
            auto text = QString(f.readAll());
            f.close();
            ui->edit_code->setPlainText(text);
            auto lf_count = text.count('\n');
            auto crlf_count = text.count("\r\n");
            if (lf_count > crlf_count * 2)
            {
                m_btn_end_of_line->setText("LF");
            }
            else
            {
                m_btn_end_of_line->setText("CRLF");
            }
        }
        else
        {
            QMessageBox::warning(this, "警告", "文件打开失败!\n" + f.errorString());
        }
    }
    else
    { // 文件不存在
        on_edit_path_textChanged("");
        SETTINGS().setValue(_APP_OPEN_FILE_, "");
    }
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::initResultTab()
{
    // 初始化结果显示标签
    m_result_tab->setWindowTitle("AVS8086 结果视图");
    m_result_tab->setWindowFlags(this->windowFlags() | Qt::Window | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
    // 置顶
    // m_result_tab->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    m_result_tab->resize(600, 400);

    m_result_1 = new QTextBrowser;
    m_result_2 = new QTextBrowser;
    m_result_3 = new QTextBrowser;
    m_result_tab->addTab(m_result_1, "词法分析");
    m_result_tab->addTab(m_result_2, "语法分析");
    m_result_tab->addTab(m_result_3, "二进制流");

    m_result_1->setOpenLinks(false);
    m_result_2->setOpenLinks(false);
    m_result_3->setOpenLinks(false);

    m_result_1->setWindowFlags(m_result_1->windowFlags() | Qt::WindowDoesNotAcceptFocus);
    m_result_2->setWindowFlags(m_result_2->windowFlags() | Qt::WindowDoesNotAcceptFocus);
    m_result_3->setWindowFlags(m_result_3->windowFlags() | Qt::WindowDoesNotAcceptFocus);

    connect(m_result_1, &QTextBrowser::anchorClicked, this, &MainWidget::onAnchorClicked);
    connect(m_result_2, &QTextBrowser::anchorClicked, this, &MainWidget::onAnchorClicked);
    connect(m_result_3, &QTextBrowser::anchorClicked, this, &MainWidget::onAnchorClicked);

    connect(ui->btn_result, &QPushButton::clicked, [this]() {
        m_result_tab->setVisible(!m_result_tab->isVisible());
        if (m_result_tab->isVisible())
        {
            m_result_tab->raise();
        }
    });
    connect(this, &QObject::destroyed, m_result_tab, &QWidget::deleteLater);
}

/* ========================================================================== */

void MainWidget::initCodeEdit()
{
    m_edit = new CodeEdit;

    // code line
    loadCodeLineColor();
    setCodeLineWidth(1);                                                          // 初始化 codeLine 的宽度
    ui->edit_line->document()->setDefaultTextOption(QTextOption(Qt::AlignRight)); // 右对齐
    ui->edit_line->viewport()->setCursor(Qt::ArrowCursor);                        // 箭头光标
    ui->edit_line->setCurrentCharFormat(m_color_f_1);                             // 设置行号的初始颜色
    ui->edit_line->appendPlainText("1");                                          // 默认值，不能用 set
    // setFocusLine(0);                                // 初始化焦点为第一行

    // ...

    // code edit
    // 改用 CodeEdit 并重新绑定信号槽
    auto i = ui->layout_code->indexOf(ui->edit_code);
    ui->layout_code->removeWidget(ui->edit_code);
    ui->edit_code->disconnect();
    ui->edit_code->deleteLater();
    ui->edit_code = m_edit;
    ui->layout_code->insertWidget(i, m_edit);
    m_edit->appendPlainText("");
    m_edit->setEditTabSize(m_char_width, 4);
    connect(m_edit, &CodeEdit::textChanged, this, &MainWidget::on_edit_code_textChanged);
    connect(m_edit, &CodeEdit::blockCountChanged, this, &MainWidget::on_edit_code_blockCountChanged);
    connect(m_edit, &CodeEdit::cursorPositionChanged, this, &MainWidget::on_edit_code_cursorPositionChanged);
    connect(m_edit, &CodeEdit::selectionChanged, this, &MainWidget::on_edit_code_cursorPositionChanged);

    // 通过信号槽将行号与编辑器同步
    connect(m_edit->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int value) {
        ui->edit_line->verticalScrollBar()->setValue(value);
        if (ui->edit_line->verticalScrollBar()->maximum() < m_edit->verticalScrollBar()->maximum())
        {
            ui->edit_line->verticalScrollBar()->setMaximum(m_edit->verticalScrollBar()->maximum());
        }
    });
    connectCodeLineChangedWithEdit();
}

/* ========================================================================== */

class CustomCompleterDelegate : public QStyledItemDelegate
{
public:
    CustomCompleterDelegate(QPushButton* btn, QObject* parent = nullptr)
        : QStyledItemDelegate(parent), m_watched(btn)
    {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        if (index.data().toString() == m_watched->text())
        { // 为特定选项设置背景颜色
            painter->fillRect(option.rect, QColor(173, 216, 230));
        }

        QStyledItemDelegate::paint(painter, option, index);
    }

private:
    QPushButton* m_watched;
};

void MainWidget::initLineSearch()
{
    // 改用 SearchEdit
    m_search = new SearchEdit;
    m_search->setStyleSheet(ui->edit_search->styleSheet());
    auto i = ui->layout_top_3->indexOf(ui->edit_search);
    ui->layout_top_3->removeWidget(ui->edit_search);
    ui->edit_search->deleteLater();
    ui->edit_search = m_search;
    ui->layout_top_3->insertWidget(i, m_search);

    // 创建输入框和补全器
    QStringList items = {"LF", "CRLF"};
    auto completer = new QCompleter(items, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);

    // 设置自定义委托
    QAbstractItemView* popup = completer->popup();
    popup->setItemDelegate(new CustomCompleterDelegate(m_btn_end_of_line, popup));
    popup->installEventFilter(m_search);
    m_btn_end_of_line->setText("CRLF"); // 设置默认值

    // 连接activated信号到槽函数
    connect(
        completer,
        QOverload<const QString&>::of(&QCompleter::activated),
        this,
        &MainWidget::editEndOfLineChanged
    );

    // connect(m_search, &QLineEdit::textChanged, this, [this]() { m_search->clear(); });

    m_search->setCompleter(completer);
}

/* ========================================================================== */

void MainWidget::initStatusBar()
{
    ui->layout_widget->addWidget(m_status);

    // 将按钮添加到状态栏的最右侧
    m_status->addPermanentWidget(m_btn_end_of_line);
    connect(m_btn_end_of_line, &QPushButton::clicked, this, [this]() { ui->edit_search->setFocus(); });

    //     QAction* action = new QAction("Click me", this);
    //     QToolButton* button = new QToolButton;
    //     button->setDefaultAction(action);

    //     // 如果想添加下拉菜单
    //     QMenu* menu = new QMenu(button);
    //     menu->addAction("Option 1");
    //     menu->addAction("Option 2");
    //     button->setMenu(menu);
    //     button->setPopupMode(QToolButton::InstantPopup);

    //     // 添加按钮到状态栏的最右侧
    //     status->addPermanentWidget(button);
}

void MainWidget::loadCodeLineColor()
{
    // 从配置文件中读取颜色
    auto color = QColor(SETTINGS().value(_APP_CL_COLOR_1_).toString());
    if (!color.isValid())
    {
        color = CODE_LINE_DEFAULT_COLOR_1;
        SETTINGS().setValue(_APP_CL_COLOR_1_, color.name());
    }
    m_color_f_1.setForeground(color);
    if (color = QColor(SETTINGS().value(_APP_CL_COLOR_2_).toString()); !color.isValid())
    {
        color = CODE_LINE_DEFAULT_COLOR_2;
        SETTINGS().setValue(_APP_CL_COLOR_2_, color.name());
    }
    m_color_f_2.setForeground(color);
}

void MainWidget::setFocusLine(int line_num)
{
    Q_ASSERT(line_num >= 0 && line_num != m_focus_line);

    if (m_focus_line != -1)
    { // 不可采用存储 QTextCursor 的方式，它内部是共享指针，可能会发生改变，产生不可预知的问题
        auto prev = QTextCursor(ui->edit_line->document()->findBlockByNumber(m_focus_line));
        prev.select(QTextCursor::BlockUnderCursor);
        prev.mergeCharFormat(m_color_f_1);
        prev.clearSelection();
    }

    auto curr = QTextCursor(ui->edit_line->document()->findBlockByNumber(line_num));
    if (line_num != 0 && curr.blockNumber() == 0)
    {                      // 当前行还未被构造，暂时放弃
        m_focus_line = -1; // 标记为 -1 交给后面主动调用
    }
    else
    {
        // 选中当前行并设置颜色
        curr.select(QTextCursor::BlockUnderCursor);
        curr.mergeCharFormat(m_color_f_2);
        curr.clearSelection();
        m_focus_line = line_num;
    }
}

void MainWidget::setCodeLineWidth(int bit_num)
{
    // 设置 codeLine 的宽度 刷新时可能会取消掉横线滚动栏
    ui->edit_line->setMaximumWidth(m_char_width * (bit_num + CODE_LINE_SPACE_WIDTH));
    // 如果 edit 水平滚动条需要显示 则显示
}

void MainWidget::saveEditTextTo(const QString& path)
{
    if (auto dir = QFileInfo(path).dir(); !dir.exists())
    { // 文件夹不存在
        if (!dir.mkpath("."))
        {
            QMessageBox::warning(this, "警告", "路径创建失败:\n" + dir.absolutePath());
            return;
        }
    }

    QFile f(path);

    if (!f.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "警告", "文件打开失败!\n" + f.errorString());
        return;
    }

    // 优化文件结构 删除行尾空白 文件以空行结尾
    auto cursor = m_edit->textCursor();
    auto doc = m_edit->document();
    cursor.beginEditBlock();
    for (auto block = doc->firstBlock(); block != doc->end(); block = block.next())
    {
        static const QRegularExpression regex("\\s+$");
        cursor.setPosition(block.position());
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);

        QString text = cursor.selectedText();
        text.remove(regex);
        if (text != cursor.selectedText())
        {
            cursor.insertText(text);
        }
    }
    // 保证文件以空行结尾
    cursor.movePosition(QTextCursor::End);
    // 如果最后一个块不是空块，则添加一个新块
    if (!cursor.block().text().isEmpty())
    {
        cursor.insertBlock();
    }
    cursor.endEditBlock();

    auto text = m_edit->toPlainText();
    text.replace("\r\n", "\n");

    // 设置写入文件的行尾符号
    if (m_btn_end_of_line->text() == "LF")
    {
    }
    else if (m_btn_end_of_line->text() == "CRLF")
    {
        text.replace("\n", "\r\n");
    }

    f.write(text.toUtf8());
    f.close();
}

// 以下为信号槽部分
/* ========================================================================== */

void MainWidget::connectCodeLineChangedWithEdit()
{ // 连接行号与编辑器的滚动条
    connect(
        ui->edit_line->verticalScrollBar(), &QScrollBar::valueChanged, this, &::MainWidget::syncLineNumToEdit
    );
}

void MainWidget::disconnectCodeLineChangedWithEdit()
{ // 断开行号与编辑器的滚动条的同步
    ui->edit_line->verticalScrollBar()->disconnect(this);
}

void MainWidget::on_edit_path_textChanged(const QString& path)
{
    bool valid = !path.isEmpty();
    ui->btn_open_file->setEnabled(valid);
    ui->btn_save_file->setEnabled(valid);
    ui->btn_clear_path->setEnabled(valid);
}

void MainWidget::on_btn_choose_path_clicked()
{
    auto fp = QFileDialog::getOpenFileName(this, "选择文件", QFileInfo(ui->edit_path->text()).filePath());
    if (!fp.isEmpty())
    { // 只判断字符串不为空，是否存在交给打开时判断
        ui->edit_path->setText(fp);
        SETTINGS().setValue(_APP_OPEN_FILE_, fp);
    }
}

void MainWidget::on_btn_clear_path_clicked()
{
    if (QMessageBox::question(this, "询问", "是否确认要清空文件路径") == QMessageBox::Yes)
    {
        ui->edit_path->clear();
        SETTINGS().setValue(_APP_OPEN_FILE_, "");
    }
}

void MainWidget::on_btn_open_file_clicked()
{
    const auto& path = ui->edit_path->text();
    if (path.isEmpty())
    {
        QMessageBox::warning(this, "警告", "路径为空或者无法访问");
        return;
    }

    if (QMessageBox::question(this, "询问", "是否打开此文件:\n" + path) == QMessageBox::Yes)
    {
        if (QFile f(path); f.exists())
        { // 文件存在
            if (f.open(QIODevice::ReadOnly | QIODevice::Text))
            { // 文件打开成功
                auto text = QString(f.readAll());
                f.close();
                ui->edit_code->setPlainText(text);
                auto lf_count = text.count('\n');
                auto crlf_count = text.count("\r\n");
                if (lf_count > crlf_count * 2)
                {
                    m_btn_end_of_line->setText("LF");
                }
                else
                {
                    m_btn_end_of_line->setText("CRLF");
                }
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

void MainWidget::on_btn_clear_file_clicked()
{
    const auto& path = ui->edit_path->text();
    if (QMessageBox::question(this, "询问", "是否确认要清空编辑器") == QMessageBox::Yes)
    {
        ui->edit_code->clear();
    }
}

void MainWidget::on_btn_save_file_clicked()
{
    const auto& path = ui->edit_path->text();

    if (QMessageBox::question(this, "询问", "是否保存文件至:\n" + path) == QMessageBox::Yes)
    {
        saveEditTextTo(path);
    }
}

void MainWidget::on_btn_save_file_as_clicked()
{
    auto fp = QFileDialog::getSaveFileName(this, "选择文件", QFileInfo(ui->edit_path->text()).filePath());
    if (!fp.isEmpty())
    { // 只判断字符串不为空，是否存在交给打开时判断
        saveEditTextTo(fp);
    }
}


void MainWidget::on_edit_code_cursorPositionChanged()
{
    auto ec = m_edit->textCursor();
    auto show_text = QString("[ row: %1, col: %2 ]").arg(ec.blockNumber() + 1).arg(ec.columnNumber() + 1);
    if (ec.hasSelection())
    {
        // 获取选中的字符数量和行数
        auto start = ec.selectionStart();
        auto end = ec.selectionEnd();
        auto char_num = end - start;
        auto show_sel_text = QString("sel: %1").arg(char_num);
        auto start_block = m_edit->document()->findBlock(start);
        auto end_block = m_edit->document()->findBlock(end);
        auto line_num = end_block.blockNumber() - start_block.blockNumber();
        if (line_num != 0 || (end_block.text().size() == end - start))
        {
            line_num += !ec.selectedText().endsWith(QChar(0x2029));
            show_sel_text += QString(", line %1").arg(line_num);
        }
        show_text += QString("( %1 )").arg(show_sel_text);
    }

    // 在状态栏显示行列 选择字符数
    m_status->showMessage(show_text);

    // 设置焦点行突出
    // 注意：cursorPositionChanged 早于 blockCountChanged
    // 如果时在最后一行，得先创建 Block 再设置，交给 on_textEdit_blockCountChanged
    if (ec.blockNumber() != m_focus_line)
    {
        setFocusLine(ec.blockNumber());
    }
}

void MainWidget::on_edit_code_blockCountChanged(int blockCount)
{
    // 显示行号 (增加 or 减少)
    auto line = ui->edit_line->blockCount();
    // if (line == blockCount)
    //     return;
    Q_ASSERT(line != blockCount);

    // 设置行号一侧的宽度
    if (auto bit_num = QString::number(blockCount).size(); QString::number(line).size() != bit_num)
    {
        setCodeLineWidth(bit_num);
    }

    disconnectCodeLineChangedWithEdit();
    { // 为避免同步滚动条的频繁操作，消耗资源，暂时屏蔽 line change with edit 的信号槽
        auto tc = ui->edit_line->textCursor();
        tc.beginEditBlock();
        tc.movePosition(QTextCursor::End);
        if (line < blockCount)
        { // 增加  <
            // 确保最后新增的块颜色是正确的
            tc.select(QTextCursor::BlockUnderCursor);
            tc.mergeCharFormat(m_color_f_1);

            for (int i = line + 1; i <= blockCount; i++)
            {
                ui->edit_line->appendPlainText(QString::number(i));
            }
        }
        else
        { // 减少  >
            while (tc.blockNumber() >= blockCount)
            {
                // blockNumber 比 blockCount 小 1
                // tc.movePosition(QTextCursor::End); // 将光标移动到文本末尾 (应该一直保持在末尾)
                tc.select(QTextCursor::BlockUnderCursor); // 选择当前块
                tc.removeSelectedText();
            }
        }
        tc.endEditBlock();
    }
    connectCodeLineChangedWithEdit();
    syncLineNumToEdit(ui->edit_line->verticalScrollBar()->value());

    // 如果光标在最后一行，得由此设置焦点 (m_focusLine 的 -1 由 setFocusLine 主动标记)
    if (m_focus_line == -1)
    {
        setFocusLine(blockCount - 1);
    }
}

// TODO: ...
void MainWidget::on_edit_code_textChanged() {}

void MainWidget::syncLineNumToEdit(int lineV)
{ // 同步行号与编辑器的滚动条
    if (int editV = m_edit->verticalScrollBar()->value(); editV != lineV)
    {
        m_edit->verticalScrollBar()->setValue(lineV);
    }
}

void MainWidget::editEndOfLineChanged(const QString& text)
{
    m_search->clearFocus();
    if (text == "LF")
    {
        m_btn_end_of_line->setText("LF");
    }
    else if (text == "CRLF")
    {
        m_btn_end_of_line->setText("CRLF");
    }
}

void MainWidget::onAnchorClicked(const QUrl& link)
{
    if (link.scheme() == "cursor")
    {
        auto pos = link.toString().remove("cursor:").split(",");
        if (pos.size() == 3)
        {
            auto row = pos[0].toInt() - 1;
            auto col = pos[1].toInt() - 1;
            auto len = pos[2].toInt();
            auto cursor = m_edit->textCursor();
            cursor.setPosition(m_edit->document()->findBlockByNumber(row).position());
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, col);
            cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, len);
            m_edit->setFocus();
            m_edit->raise();
            m_edit->activateWindow();

            m_edit->setTextCursor(cursor);
            m_edit->ensureCursorVisible();
        }
    }
    else if (link.scheme() == "file")
    {
        QString filePath = link.toLocalFile();
        bool success = QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        if (!success)
        {
            qDebug() << "Failed to open file:" << filePath;
        }
    }
}

// 下面部分为功能测试
/* ========================================================================== */

void MainWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        m_result_tab->hide();
        return;
    }

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_S)
    {
        on_btn_save_file_clicked();
        return;
    }

    QWidget::keyPressEvent(event);
}

#include "common/json.h"
#include "lexer/lexer.h"
#include "parser/parser.h"

void MainWidget::on_btn_lexer_clicked()
{
    // 清空结果显示 (词法分析)
    m_result_1->clear();
    // 设置显示词法分析页
    m_result_tab->raise();
    if (!m_result_tab->isVisible())
    {
        m_result_tab->show();
    }
    if (m_result_tab->currentIndex() != 0)
    {
        m_result_tab->setCurrentIndex(0);
    }

    using namespace lzl::a1;
    using namespace lzl::a1::lexer;
    Lexer lexer(ui->edit_path->text());

    // 输出词法分析结果
    static const QString p_fmt = R"(<a href="cursor:%1,%2,%3">%1,%2,%3</a>)";
    {
        const auto tokens = lexer.tokens();
        qsizetype rs_max = 0;
        qsizetype cs_max = 0;
        qsizetype ls_max = 0;
        for (const auto& t : tokens)
        {
            rs_max = qMax(rs_max, QString::number(t.row()).size());
            cs_max = qMax(cs_max, QString::number(t.column()).size());
            ls_max = qMax(ls_max, QString::number(t.length()).size());
        }
        static const QString t_fmt = R"([%1] >>>  %2)";
        for (auto& t : tokens)
        {
            auto pos_str(p_fmt.arg(t.row(), rs_max).arg(t.column(), cs_max).arg(t.length(), ls_max));
            m_result_1->append(t_fmt.arg(pos_str, t.content()));
        }
        auto lt = lexer.end();
        m_result_1->append(t_fmt.arg(
            p_fmt.arg(lt.row(), rs_max).arg(lt.column(), cs_max).arg(lt.length(), ls_max), lt.content()
        ));
        m_result_1->append("");
    }

    // 输出此法分析日志
    {
        const auto infos = lexer.infos();
        qsizetype ts_max = 0;
        qsizetype rs_max = 0;
        qsizetype cs_max = 0;
        qsizetype ls_max = 0;
        for (const auto& e : infos)
        {
            ts_max = qMax(ts_max, e.typeName().size());
            rs_max = qMax(rs_max, QString::number(e.pos().row()).size());
            cs_max = qMax(cs_max, QString::number(e.pos().column()).size());
            ls_max = qMax(ls_max, QString::number(e.pos().length()).size());
        }

        auto centerAlign = [](const QString& str, qsizetype max) {
            auto mid = (max - str.size()) / 2;
            return str.leftJustified(mid + str.size(), ' ').rightJustified(max, ' ');
        };

        for (const auto& e : infos)
        {
            QMap<Info::Type, QString> colors = {
                {Info::INFORMATION, "black" },
                {Info::SUGGESTION,  "blue"  },
                {Info::WARNING,     "orange"},
                {Info::ERROR,       "red"   },
            };

            auto pos_str(
                p_fmt.arg(e.pos().row(), rs_max).arg(e.pos().column(), cs_max).arg(e.pos().length(), ls_max)
            );
            static const QString e_fmt = R"(<span style="color:%1;">[%2]</span> [%3] >>>  %4)";

            m_result_1->append(
                e_fmt.arg(colors[e.type()], centerAlign(e.typeName(), ts_max)).arg(pos_str, e.value())
            );
        }
        m_result_1->append("");
    }
}

void MainWidget::on_btn_parser_clicked()
{
    // 清空结果显示 (语法分析)
    m_result_2->clear();
    // 设置显示语法分析页
    m_result_tab->raise();
    if (!m_result_tab->isVisible())
    {
        m_result_tab->show();
    }
    if (m_result_tab->currentIndex() != 1)
    {
        m_result_tab->setCurrentIndex(1);
    }

    using namespace lzl::a1;
    using namespace lzl::a1::lexer;
    using namespace lzl::a1::parser;
    Lexer lexer(ui->edit_path->text());
    Parser parser(&lexer);
    auto root = parser.newAST();

    // 输出语法分析日志
    {
        static const QString p_fmt = R"(<a href="cursor:%1,%2,%3">%1,%2,%3</a>)";

        const auto infos = parser.infos();
        qsizetype ts_max = 0;
        qsizetype rs_max = 0;
        qsizetype cs_max = 0;
        qsizetype ls_max = 0;
        for (const auto& e : infos)
        {
            ts_max = qMax(ts_max, e.typeName().size());
            rs_max = qMax(rs_max, QString::number(e.pos().row()).size());
            cs_max = qMax(cs_max, QString::number(e.pos().column()).size());
            ls_max = qMax(ls_max, QString::number(e.pos().length()).size());
        }

        auto centerAlign = [](const QString& str, qsizetype max) {
            auto mid = (max - str.size()) / 2;
            return str.leftJustified(mid + str.size(), ' ').rightJustified(max, ' ');
        };

        for (const auto& e : infos)
        {
            QMap<Info::Type, QString> colors = {
                {Info::INFORMATION, "black" },
                {Info::SUGGESTION,  "blue"  },
                {Info::WARNING,     "orange"},
                {Info::ERROR,       "red"   },
            };

            auto pos_str(
                p_fmt.arg(e.pos().row(), rs_max).arg(e.pos().column(), cs_max).arg(e.pos().length(), ls_max)
            );
            static const QString e_fmt = R"(<span style="color:%1;">[%2]</span> [%3] >>>  %4)";

            m_result_2->append(
                e_fmt.arg(colors[e.type()], centerAlign(e.typeName(), ts_max)).arg(pos_str, e.value())
            );
        }
    }

    // 输出 AST
    {
        Json js;
        js.setObject(root->json());

        QDir dir(DOCS_PATH);
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
        auto json_path = QDir::cleanPath(dir.absoluteFilePath("ast.json"));
        js.saveToFile(json_path);

        m_result_2->append("");
        static const QString p_fmt = R"(ast.json save as: <a href="file:///%1">%1</a>)";
        m_result_2->append(p_fmt.arg(json_path));
    }
}

void MainWidget::on_btn_to_bin_clicked()
{
    // 清空结果显示 (二进制流)
    m_result_3->clear();
    // 设置显示二进制流页
    m_result_tab->raise();
    if (!m_result_tab->isVisible())
    {
        m_result_tab->show();
    }
    if (m_result_tab->currentIndex() != 2)
    {
        m_result_tab->setCurrentIndex(2);
    }

    using namespace lzl::a1;
    using namespace lzl::a1::lexer;
    using namespace lzl::a1::parser;
    Lexer lexer(ui->edit_path->text());
    Parser parser(&lexer);
    auto root = parser.newAST();

    // 输出二进制流
    {
        QByteArray codes;
        const auto stmts = root->stmts();
        for (const auto& stmt : stmts)
        {
            codes.append(stmt->code());
        }
        m_result_3->append(codes.toHex(' ').toUpper());
    }
}

// #include "service/json.h"
// #include "lexer/lexer.h"
// #include "parser/parser.h"
// // #include "assembler/assembler.h"
// // #include "vm/vm.h"

// void MainWidget::on_btn_compile_clicked()
// {
//     using namespace avs8086;
//     using namespace avs8086::token;
//     using namespace avs8086::lexer;
//     using namespace avs8086::parser;
//     // using namespace avs8086::assembler;
//     // using namespace avs8086::vm;

//     Lexer l(ui->lineEdit->text());
//     for (const auto& t : l.tokens())
//         qDebug().noquote() << QString("[ %1, %2, %3 ] >> %4")
//             .arg(t.row(), 2).arg(t.column(), 2).arg(t.length(), 2).arg(t.content());
//     auto lt(l.end());
//     qDebug().noquote() << QString("[ %1, %2, %3 ] >> %4")
//         .arg(lt.row(), 2).arg(lt.column(), 2).arg(lt.length(), 2).arg(lt.content());
//     // qDebug() << "error:";
//     // for (const auto& e : l.infos())
//     //     qDebug() << e.typeName()
//     //              << e.position().row()
//     //              << e.position().column()
//     //              << e.position().length()
//     //              << e.value();
//     // qDebug().noquote() << l.restore(l.tokens());

//     Parser p(&l);
//     auto root = p.newAST();
//     for (const auto& e : p.infos())
//         qDebug() << e.typeName() << ": ("
//                  << e.pos().row() << ","
//                  << e.pos().column() << ","
//                  << e.pos().length() << ") >"
//                  << e.value();

//     // Assembler a(root);
//     // a.compile();
//     // // for (const auto& e : a.errorInfos())
//     // //     qDebug() << e;

//     // // a.saveToFile();
//     // a.copy(R"(D:\Leisure\Desktop\8086\AVS\mycode.bin)",
//     //        R"(D:\Leisure\Desktop\8086\AVS\test.bin)");
//     // VM vm;
//     // vm.m_regs = a.wellInitInfos();
//     // vm.readFromFile(R"(D:\Leisure\Desktop\8086\AVS\test.bin)");


//     // for (const auto& s : root->traversal())
//     //     qDebug() << s;
//     // qDebug() << "error:";
//     // for (const auto& e : p.errorInfos())
//     //     qDebug() << e;
//     // qDebug() << "warning:";
//     // for (const auto& w : p.warningInfos())
//     //     qDebug() << w;

//     Json js;
//     js.setObject(root->json());
//     js.saveToFile(DOCS_PATH"/ast.json");
//     // qDebug() << js.getObject();

//     // qDebug() << a.wellInitInfos();
// }

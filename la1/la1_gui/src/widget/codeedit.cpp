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


#include "codeedit.h"

#include <QApplication>
#include <QScrollBar>
#include <QTextBlock>

CodeEdit::CodeEdit(QWidget* parent) : QPlainTextEdit(parent)
{
    this->setObjectName("CodeEdit");
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    this->setCursorWidth(2);

    setEditTabSize(13, 4); // default
}

CodeEdit::~CodeEdit() {}

void CodeEdit::setEditTabSize(int char_width, int tab_size)
{
    auto textOption = this->document()->defaultTextOption();
    textOption.setTabStopDistance(char_width * tab_size - 2);
    this->document()->setDefaultTextOption(textOption);
}

void CodeEdit::setEditTab(TabType type, int tab_size)
{
    Q_ASSERT(type == TAB_USE_TABLE || TAB_USE_SPACE);
    Q_ASSERT(tab_size > 0);
    m_tab = type;
    m_tab_size = tab_size;
}

void CodeEdit::setEditTab(TabType type, int tab_size, int char_width)
{
    Q_ASSERT(char_width > 0);
    setEditTab(type, tab_size);
    setEditTabSize(char_width, tab_size);
}

void CodeEdit::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::ShiftModifier)
    {
        // 按住Shift键时进行横向滚动
        int delta = event->angleDelta().y();
        // 获取垂直滚动值
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta);
    }
    else
    {
        // 否则进行默认的垂直滚动
        QPlainTextEdit::wheelEvent(event);
    }
}

void CodeEdit::keyPressEvent(QKeyEvent* event)
{
    this->viewport()->update(this->cursorRect());

    if (event->modifiers() == Qt::NoModifier)
    {
        this->ensureCursorVisible();
    }

    // 换行后同步缩进  按住 Shift 正常换行
    if (event->key() == Qt::Key_Return && !(event->modifiers() & ~Qt::ShiftModifier))
    {
        auto cursorFlashTimeBak = QApplication::cursorFlashTime();
        QApplication::setCursorFlashTime(0);
        // 获取当前光标
        auto cursor = this->textCursor();
        if (event->modifiers() == Qt::ShiftModifier)
        {
            // 如果按住 Shift 键则不进行缩进
            cursor.insertBlock();
            QApplication::setCursorFlashTime(cursorFlashTimeBak);
            return;
        }
        // 计算当前行的缩进
        int curr_front = 0;
        const auto& text = cursor.block().text();
        for (int i = 0; i < text.size(); i++)
        {
            if (i >= cursor.columnNumber())
            {
                break;
            }
            if (text[i] == ' ')
            {
                curr_front++;
            }
            else if (text[i] == '\t')
            {
                curr_front += m_tab_size;
            }
            else
            {
                break;
            }
        }
        curr_front /= m_tab_size;
        // 在光标位置插入新段落
        cursor.insertBlock();
        // 插入缩进
        if (m_tab == TAB_USE_TABLE)
        {
            cursor.insertText(QString(curr_front, '\t'));
        }
        else
        {
            cursor.insertText(QString(curr_front * m_tab_size, ' '));
        }
        QApplication::setCursorFlashTime(cursorFlashTimeBak);
        return;
    }

    // 如果只有缩进符 退格键删除一个缩进
    if (event->key() == Qt::Key_Backspace && event->modifiers() == Qt::NoModifier && m_tab == TAB_USE_SPACE)
    {
        auto cursor = this->textCursor();
        if (cursor.hasSelection())
        {
            QPlainTextEdit::keyPressEvent(event);
            return;
        }
        cursor.movePosition(QTextCursor::StartOfBlock);
        auto size = cursor.block().text().size();
        if (size == 0)
        {
            QPlainTextEdit::keyPressEvent(event);
            return;
        }
        for (int i = 0; i < size; i++)
        {
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            if (!cursor.selectedText().endsWith(' '))
            {
                QPlainTextEdit::keyPressEvent(event);
                return;
            }
            if (cursor.selectedText().size() == m_tab_size + 1)
            {
                cursor.clearSelection();
                cursor.movePosition(QTextCursor::Left);
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
            }
        }
        cursor.removeSelectedText();
        return;
    }

    // 选中后 ESC 取消选中
    if (event->key() == Qt::Key_Escape)
    {
        auto cursor = this->textCursor();
        cursor.clearSelection();
        this->setTextCursor(cursor);
        return;
    }

    // Tab BackTab Ctrl+/
    if (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab
        || (event->key() == Qt::Key_Slash && event->modifiers() & Qt::ControlModifier))
    {
        // 获取选中的行
        auto cursor = this->textCursor();
        auto start = cursor.selectionStart();
        auto end = cursor.selectionEnd();
        auto start_block = this->document()->findBlock(start);
        auto end_block = this->document()->findBlock(end);
        auto line_num = end_block.blockNumber() - start_block.blockNumber();

        cursor.beginEditBlock(); // 确保这一组操作的原子性
        if (event->key() == Qt::Key_Tab)
        {
            if (cursor.hasSelection() && (line_num != 0 || (end_block.text().size() == end - start)))
            {
                line_num += 1 + !cursor.selectedText().endsWith(QChar(0x2029));
                for (auto block = start_block; (--line_num); block = block.next())
                {
                    cursor.setPosition(block.position());
                    cursor.movePosition(QTextCursor::StartOfBlock);

                    // 使用 Table 缩进时的处理
                    if (m_tab == TAB_USE_TABLE)
                    {
                        cursor.insertText("\t");
                    }
                    // 使用 Space 缩进时的处理
                    else
                    {
                        // 扫描每组的 space tab 直到不是空格为止
                        for (int i = 0; i < block.text().size(); i++)
                        {
                            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                            if (cursor.selectedText().size() == m_tab_size)
                            {
                                cursor.clearSelection();
                            }
                            if (cursor.hasSelection() && !cursor.selectedText().endsWith(' '))
                            {
                                cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
                                break;
                            }
                        }
                        cursor.insertText(QString(m_tab_size, ' '));
                    }
                }
            }
            else
            {
                if (m_tab == TAB_USE_TABLE)
                {
                    cursor.insertText("\t");
                }
                else
                {
                    cursor.insertText(QString(m_tab_size - (cursor.columnNumber() % m_tab_size), ' '));
                }
            }
        }
        else if (event->key() == Qt::Key_Backtab)
        {
            // 遍历每一行
            line_num += 1 + !cursor.selectedText().endsWith(QChar(0x2029));
            for (auto block = start_block; (--line_num); block = block.next())
            {
                cursor.setPosition(block.position());
                cursor.movePosition(QTextCursor::StartOfBlock);

                // 使用 Table 缩进时的处理
                if (m_tab == TAB_USE_TABLE)
                {
                    // cursor 向后选择一个字符
                    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                    // 如果选中的字符是 Tab 则删除
                    if (cursor.selectedText() == "\t")
                    {
                        cursor.removeSelectedText();
                    }
                }
                // 使用 Space 缩进时的处理
                else
                {
                    // 扫描每组的 space tab 直到不是空格为止
                    for (int i = 0; i < block.text().size(); i++)
                    {
                        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                        if (!cursor.selectedText().endsWith(' '))
                        {
                            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);
                            break;
                        }
                        if (cursor.selectedText().size() == m_tab_size + 1)
                        {
                            cursor.clearSelection();
                            cursor.movePosition(QTextCursor::Left);
                            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                        }
                    }
                    cursor.removeSelectedText();
                }
            }
        }
        else if (event->key() == Qt::Key_Slash && event->modifiers() & Qt::ControlModifier)
        {
            bool is_del_annotation = true;
            int front = 0x7fffffff;
            int nul_front = 0x7fffffff;
            QList<QTextCursor> cursors;
            QList<QTextCursor> nul_cursors;
            // 遍历每一行
            line_num += 1 + !cursor.selectedText().endsWith(QChar(0x2029));
            for (auto block = start_block; (--line_num); block = block.next())
            {
                cursor.setPosition(block.position());
                cursor.movePosition(QTextCursor::StartOfBlock);

                int curr_front = 0;
                // 扫描每组的 space tab 直到不是空格为止
                while (curr_front < block.text().size())
                {
                    curr_front++;
                    cursor.clearSelection();
                    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                    if (cursor.selectedText() != " " && cursor.selectedText() != "\t")
                    {
                        break;
                    }
                }
                curr_front /= m_tab_size;
                if (cursor.selectedText() == "" || cursor.selectedText() == " "
                    || cursor.selectedText() == "\t")
                {
                    nul_cursors.append(cursor);
                    if (curr_front < nul_front)
                    {
                        nul_front = curr_front;
                    }
                }
                else
                {
                    cursors.append(cursor);
                    if (curr_front < front)
                    {
                        front = curr_front;
                    }
                    if (cursor.selectedText() != ";")
                    {
                        is_del_annotation = false;
                    }
                }
            }

            // 都有的时候 忽略空行
            if (cursors.isEmpty())
            {
                cursors.swap(nul_cursors);
                front = nul_front;
                is_del_annotation = false;
            }

            // delete annotation
            if (is_del_annotation)
            {
                for (int i = 0; i < cursors.size(); i++)
                {
                    cursors[i].removeSelectedText();
                    cursors[i].movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                    if (cursors[i].selectedText() == " ")
                    {
                        cursors[i].removeSelectedText();
                    }
                }
            }
            // add annotation
            else
            {
                for (int i = 0; i < cursors.size(); i++)
                {
                    cursors[i].movePosition(QTextCursor::StartOfBlock);
                    cursors[i].movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, front);
                    cursors[i].insertText("; ");
                }
            }
        }
        cursor.endEditBlock();
        return;
    }

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        if (event->modifiers() == Qt::NoModifier)
        {

            // 修复 按住上下键时如果在首行或者尾行 会出现无法取消选择的情况
            auto cursor = this->textCursor();
            if (cursor.hasSelection())
            {
                cursor.clearSelection();
                this->setTextCursor(cursor);
            }
        }
        else if (event->modifiers() & Qt::ControlModifier)
        {
            // 按住 Ctrl 键时进行上下滚动
            int delta = event->key() == Qt::Key_Up ? -1 : 1;
            // 设置垂直滚动值
            this->verticalScrollBar()->setValue(verticalScrollBar()->value() + delta);
            return;
        }
    }

    QPlainTextEdit::keyPressEvent(event);
}

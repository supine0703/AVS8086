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

CodeEdit::CodeEdit(QWidget* parent) : QPlainTextEdit(parent)
{
    this->setObjectName("CodeEdit");
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    this->setCursorWidth(2);

    setEditTabSize(13, 4); // default
}

CodeEdit::~CodeEdit() {}

void CodeEdit::setEditTabSize(int charWidth, int tabSize)
{
    auto textOption = this->document()->defaultTextOption();
    textOption.setTabStopDistance(charWidth * tabSize);
    this->document()->setDefaultTextOption(textOption);
}

void CodeEdit::setEditTab(TabType type, int tabSize)
{
    Q_ASSERT(type == TAB_USE_TABLE || TAB_USE_SPACE);
    Q_ASSERT(tabSize > 0);
    m_tab = type;
    m_tabSize = tabSize;
}

void CodeEdit::setEditTab(TabType type, int tabSize, int charWidth)
{
    Q_ASSERT(charWidth > 0);
    setEditTab(type, tabSize);
    setEditTabSize(charWidth, tabSize);
}

void CodeEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ShiftModifier))
    { // 取消 shift+enter 只空行不创建新 block
        // 在光标位置插入新段落
        auto cursor = this->textCursor();
        cursor.insertBlock();
    }
    else if (event->key() == Qt::Key_Tab /*&& !(event->modifiers() & Qt::ShiftModifier)*/)
    { // tab
        if (m_tab == TAB_USE_SPACE)
        {
            auto cursor = this->textCursor();
            cursor.insertText(QString(m_tabSize - cursor.columnNumber() % m_tabSize, ' '));
        }
        else
        {
            QPlainTextEdit::keyPressEvent(event);
        }
    }
    else if (event->key() == Qt::Key_Backtab)
    { // shift + tab
        auto cursor = this->textCursor();
        cursor.beginEditBlock();
        { // 确保这一组操作的原子性
            auto cursorCol = cursor.columnNumber();
            cursor.movePosition(QTextCursor::StartOfLine);
            auto startPos = cursor.position();
            cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
            auto lineText = cursor.selectedText();

            if (m_tab == TAB_USE_SPACE)
            { // 使用空格缩进时的处理
                int space = 0;
                while (space < cursorCol && lineText[space] == ' ')
                {
                    ++space;
                }
                if (space > 0)
                {
                    cursor.setPosition(startPos);
                    for (space %= m_tabSize, space += (!space) ? 4 : 0; space > 0; space--)
                    {
                        cursor.deleteChar();
                    }
                }
            }
            else
            { // 使用 Tab 缩进时的处理
                if (cursorCol > 0 && lineText.startsWith("\t"))
                {
                    cursor.setPosition(startPos);
                    cursor.deleteChar();
                }
            }
        }
        cursor.endEditBlock();
    }

    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}

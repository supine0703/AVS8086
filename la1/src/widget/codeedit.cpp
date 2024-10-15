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

    QTextDocument *doc = this->document();
    QTextCursor cursor(doc);
    cursor.select(QTextCursor::Document); // 选择整个文档

    QTextBlockFormat blockFormat;
    blockFormat.setLineHeight(40, QTextBlockFormat::SingleHeight); // 设置固定行高
    cursor.setBlockFormat(blockFormat);
}

CodeEdit::~CodeEdit() {}

void CodeEdit::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return && (event->modifiers() & Qt::ShiftModifier))
    { // 取消 shift+enter 只空行不创建新 block
        // 在光标位置插入新段落
        QTextCursor cursor = this->textCursor();
        cursor.insertBlock();
    }

    else
    {
        QPlainTextEdit::keyPressEvent(event);
    }
}

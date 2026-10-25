#include "searchedit.h"

#include <QCompleter>

SearchEdit::SearchEdit(QWidget* parent) : QLineEdit(parent) {}

void SearchEdit::focusOutEvent(QFocusEvent* event)
{
    this->clear();
    QLineEdit::focusOutEvent(event);
}

void SearchEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);
    this->completer()->setCompletionPrefix("");
    this->completer()->complete(); // 显示下拉选项
}


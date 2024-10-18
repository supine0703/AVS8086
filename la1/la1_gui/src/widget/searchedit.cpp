#include "searchedit.h"

#include <QCompleter>

SearchEdit::SearchEdit(QWidget* parent)
    : QLineEdit(parent)
{

}

void SearchEdit::focusOutEvent(QFocusEvent* event)
{
    QLineEdit::focusOutEvent(event);
    this->clear();
}

void SearchEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);
    completer()->complete(); // 显示下拉选项
}


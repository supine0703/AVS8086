#include "codeedit.h"

CodeEdit::CodeEdit(QWidget* parent) : QPlainTextEdit(parent)
{
    this->setObjectName("CodeEdit");
}

CodeEdit::~CodeEdit()
{
}

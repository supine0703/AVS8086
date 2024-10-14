#ifndef CODE_EDIT_H
#define CODE_EDIT_H

#include <QPlainTextEdit>

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEdit(QWidget* parent = nullptr);
    ~CodeEdit();


};

#endif // CODE_EDIT_H

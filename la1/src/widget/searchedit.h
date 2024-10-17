#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QLineEdit>

class SearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    SearchEdit();

    // QWidget interface
protected:
    virtual void focusOutEvent(QFocusEvent* event) override;

    virtual void focusInEvent(QFocusEvent* event) override;
};

#endif // SEARCHEDIT_H

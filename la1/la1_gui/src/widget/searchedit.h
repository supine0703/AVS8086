#ifndef SEARCHEDIT_H
#define SEARCHEDIT_H

#include <QLineEdit>

class SearchEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit  SearchEdit(QWidget* parent = nullptr);

    // QWidget interface
protected:
    virtual void focusOutEvent(QFocusEvent* event) override;

    virtual void focusInEvent(QFocusEvent* event) override;
};

#endif // SEARCHEDIT_H

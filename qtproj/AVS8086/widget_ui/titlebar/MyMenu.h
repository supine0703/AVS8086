#ifndef MYMENU_H
#define MYMENU_H
#include <QMenu>

class MyMenu : public QMenu
{
public:
    explicit MyMenu(QWidget* parent = nullptr);
    explicit MyMenu(const QString &title, QWidget *parent = nullptr);
    ~MyMenu();

protected:
    void showEvent(QShowEvent* event) override;//重写showWidget
};

#endif // MYMENU_H

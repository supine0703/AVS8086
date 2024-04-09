#include "MyMenu.h"
#include <QPropertyAnimation>

MyMenu::MyMenu(QWidget* parent):
    QMenu(parent)
{
}

MyMenu::MyMenu(const QString &title, QWidget *parent):
    QMenu(title, parent)
{

}

void MyMenu::showEvent(QShowEvent* event)
{
    QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(250);/// Set the time, expressed in ms
    animation->setStartValue(QRect(this->x(), this->y(), 0, this->height()));
    animation->setEndValue(QRect(this->x(), this->y(), this->width(), this->height()));
    animation->start(QAbstractAnimation::KeepWhenStopped);//动画结束后不会被删除

    QMenu::showEvent(event);//用来确保窗口显示正常
}
MyMenu::~MyMenu()
{

}

#ifndef VMOBJECT_H
#define VMOBJECT_H

#include <QObject>

class VMObject : public QObject
{
    Q_OBJECT
public:
    explicit VMObject(QObject* parent = nullptr);

signals:
};

#endif // VMOBJECT_H

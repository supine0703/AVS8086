#include "vm.h"

#include <QFileInfo>

using namespace avs8086::vm;

VM::VM()
{
}

void VM::readFromFile(const QString& file)
{
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "open fail:" << f.error();
        return;
    }
    m_codes.clear();
    m_codes.append(f.readAll());
    qDebug() << m_codes.toHex(' ');

    f.close();
}

void VM::test()
{

}

#ifndef VM_H
#define VM_H

#include "token/token.h"

namespace avs8086::vm {

class VM
{
public:
    VM();

    void readFromFile(const QString& file);

    void test();

private:
    QString m_file;
    QByteArray m_codes;

};

} // namespace vm

#endif // VM_H

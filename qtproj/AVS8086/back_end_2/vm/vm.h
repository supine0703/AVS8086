#ifndef VM_H
#define VM_H

#include "token/token.h"

namespace avs8086::vm {

class VM
{
    enum ByteOrder {
        BigEndian,
        LittleEndian,
    };

public:
    enum RegI {
        AX = 0,
        CX,
        DX,
        BX,
        SP,
        BP,
        SI,
        DI,
        ES,
        CS,
        SS,
        DS,
        IP,
        SEGMENT,
        LOFFSET,
        SIZE,
    };
public:
    VM();

    void readFromFile(const QString& file);

    void initVM();

    void singleRun();

    quint16& REG(RegI index);


    int adder();

    ByteOrder byteOrder() const;

    QList<quint16> m_regs;
    QList<bool> m_flagReg;
    QByteArray m_memory;


private:
    QString m_file;
    QByteArray m_codes;
};

} // namespace vm

#endif // VM_H

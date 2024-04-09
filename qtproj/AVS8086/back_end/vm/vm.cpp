#include "vm.h"

#include "assembler/assembler.h"
#include <QFileInfo>

using namespace avs8086::vm;
using namespace avs8086::assembler;

VM::VM()
{
    initVM();
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
    // qDebug() << m_codes.toHex(' ');

    auto load_seg = m_regs.at(Assembler::LOAD_SEGMENT);
    auto offset  = m_regs.at(Assembler::LOAD_OFFSET);
    for (auto& b : m_codes)
    {
        // qDebug() << QString::number(load_seg * 16 + offset, 16);
        m_memory[load_seg * 16 + offset] = b;
        // qDebug() << QString::number(load_seg, 16) <<  QString::number(offset, 16)
        //          << QString::number((quint8)m_memory[load_seg * 16 + offset], 16);
        offset++;
    }

    f.close();
}

void VM::initVM()
{
    m_memory.clear();
    m_memory.resize(1048576, 0x00);
    m_flagReg.clear();
    m_flagReg.resize(16);
    m_regs.clear();
    m_regs.resize(SIZE);

    bool order = byteOrder();


}

void VM::singleRun()
{
    // auto
}

quint16& VM::REG(RegI index)
{
    return m_regs[index];
}

VM::ByteOrder VM::byteOrder() const
{
    union {
        quint16 word = 0x1234;
        quint8 byte[2];
    } test;
    if (*test.byte == 0x12)
        return BigEndian;
    return LittleEndian;
}

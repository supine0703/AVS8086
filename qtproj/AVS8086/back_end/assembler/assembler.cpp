#include "assembler.h"

#include <QFileInfo>

using namespace avs8086::assembler;

Assembler::Assembler()
    : Assembler(nullptr)
{ }

Assembler::Assembler(const QSharedPointer<ast::Program>& root)
    : m_root(root)
    , m_path("")
{
    if (root.isNull())
        return;
    QFileInfo info(root->file());
    m_file = info.baseName() + ".bin";
}

Assembler::~Assembler()
{ }

void Assembler::setOutputPath(const QString& path)
{
    m_path = path;
}

bool Assembler::saveToFile() const
{
    QString fn;
    if (m_path.isEmpty())
    {
        QFileInfo info(m_root->file());
        fn = info.path() + "/" + m_file;
    }
    else
    {
        fn = m_path + m_file;
    }

    QFile f(fn);
    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "file open false:";
        qDebug() << f.errorString();
        return false;
    }

    f.write(m_codes);

    f.close();
    return true;
}

/* ========================================================================== */

void Assembler::compile()
{
    m_codes.clear();
    for (const auto& s : m_root->m_statements)
    {
        auto it = sm_compile_fns.find(s->type());
        if (it == sm_compile_fns.end())
        {
            addErrorInfo(
                s->m_token.row(), s->m_token.column(),
                s->m_token.literal().length(),
                QString("there is can not compile instruct: %1")
                        .arg(s->typeName())
            );
        }
        else
        {
            (this->*it.value())(s);
        }
    }
}

void Assembler::addErrorInfo(int row, int col, int len, const QString& info)
{
    m_errorInfos.append(
        QString("[%1:(%3,%4,%5)]>%2")
            .arg(m_root->file(), info).arg(row).arg(col).arg(len)
        );
}

QByteArray& Assembler::appendCode(const QByteArray& bytes)
{
    m_codes.append(bytes);
    return m_codes;
}

QByteArray Assembler::binStrToByte(const QString& bin)
{
    QByteArray bytes;
    for (int i = bin.length() - 1; i >= 0; i--)
    {

    }
}


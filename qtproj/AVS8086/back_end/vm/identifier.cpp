#include "vm/identifier.h"

using namespace avs8086::ast;

Identifier::Identifier()
    : Identifier("", 0, 0, "")
{ }

Identifier::Identifier(
    const QString& id, Type type, int offset, const QString& segment)
    : m_id(id), m_offset(offset), m_type(type), m_segment(segment)
{
    Q_ASSERT(type != VAR);
    auto it = sm_typeSize.find(m_type);
    Q_ASSERT(it != sm_typeSize.end());
    m_size = it.value();
}

Identifier::Identifier(
    const QString& id, int size, int offset, const QString& segment)
    : m_id(id), m_offset(offset), m_size(size), m_segment(segment)
{
    auto it = sm_sizeType.find(m_size);
    if (it == sm_sizeType.end())
    {
        m_type = m_size > 0 ? VAR : ILLEGAL;
    }
    else
    {
        m_type = it.value();
    }
}

/* ========================================================================== */

#if 0
bool Identifier::operator==(const Identifier& other) const
{
    return this->m_id == other.m_id;
}
#endif

QString Identifier::id() const
{
    return m_id;
}

Identifier::Type Identifier::type() const
{
    return m_type;
}

int Identifier::offset() const
{
    return m_offset;
}

int Identifier::size() const
{
    return m_size;
}

QString Identifier::segment() const
{
    return m_segment;
}

/* ========================================================================== */

QString Identifier::typeName() const
{
    return typeName(m_type);
}

QString Identifier::typeName(Type type)
{
    auto it = sm_typeNames.find(type);
    if (it != sm_typeNames.end())
        return it.value();
    return "ILLEGAL";
}

/* ========================================================================== */
const QMap<Identifier::Type, int> Identifier::sm_typeSize = {
    { ILLEGAL,  0  },
    { LABEL,    -1 },
    { NEAR,     -2 },
    { FAR,      -3 },
    { SEGMENT,  -4 },
};

const QMap<int, Identifier::Type> Identifier::sm_sizeType = {
    { -1,       LABEL },
    { -2,       NEAR },
    { -3,       FAR },
    { -4,       SEGMENT },
};

const QMap<Identifier::Type, QString> Identifier::sm_typeNames = {
    { SEGMENT,  "SEGMENT" },
    { LABEL,    "LABEL" },
    { VAR,      "VAR" },
    { FAR,      "FAR" },
    { NEAR,     "NEAR" },
};

/* ========================================================================== */
/* ========================================================================== */

bool IdentifierList::contains(const QString& id) const
{
    return m_set.contains(id);
}

bool IdentifierList::append(const Identifier& identifier)
{
    auto id = identifier.id();
    if (contains(id))
        return false;
    m_list.append(identifier);
    return true;
}

QList<Identifier> IdentifierList::list() const
{
    return m_list;
}

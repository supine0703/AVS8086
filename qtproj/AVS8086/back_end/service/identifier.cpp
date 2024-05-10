#include "identifier.h"

using namespace avs8086;

/* ========================================================================== */

Identifier::Identifier(
    const QString& id, Type type, int offset, const QString& segment)
    : m_id(id)
    , m_offset(offset)
    , m_type((Q_ASSERT(type != VAR), type))
    , m_segment(segment)
{
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

QString Identifier::typeName(Type type)
{
    auto it = sm_typeNames.find(type);
    if (it != sm_typeNames.end())
        return it.value();
    return "ILLEGAL";
}

/* ========================================================================== */
const QHash<Identifier::Type, int> Identifier::sm_typeSize = {
    { ILLEGAL,  0  },
    { LABEL,    -1 },
    { NEAR,     -2 },
    { FAR,      -3 },
    { SEGMENT,  -4 },
};

const QHash<int, Identifier::Type> Identifier::sm_sizeType = {
    { -1,       LABEL },
    { -2,       NEAR },
    { -3,       FAR },
    { -4,       SEGMENT },
};

const QHash<Identifier::Type, QString> Identifier::sm_typeNames = {
    { SEGMENT,  "SEGMENT" },
    { LABEL,    "LABEL" },
    { VAR,      "VAR" },
    { FAR,      "FAR" },
    { NEAR,     "NEAR" },
};

/* ========================================================================== */
/* ========================================================================== */

#if 0
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
#endif

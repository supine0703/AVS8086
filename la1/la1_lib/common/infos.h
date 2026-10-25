#ifndef INFOS_H
#define INFOS_H

#include "position.h"
#include "utils/map_and_set.h"

namespace lzl::a1 {

class Info
{
public:
    enum Type {
        INFORMATION = 0,
        SUGGESTION,
        WARNING,
        ERROR,
    };

private:
    struct Key
    {
    public:
        Key() : m_type(INFORMATION) {}
        Key(Type type, const Position& pos) : m_type(type), m_pos(pos) {}
        ~Key() = default;

#if __cplusplus >= 202002L
        friend auto operator<=>(const Key&, const Key&) = default;
#else
        friend bool operator<(const Key& k1, const Key& k2)
        {
            if (k1.m_type != k2.m_type)
            {
                return k1.m_type < k2.m_type;
            }
            return k1.m_pos < k2.m_pos;
        }

        friend bool operator==(const Key& k1, const Key& k2)
        {
            return k1.m_type == k2.m_type && k1.m_pos == k2.m_pos;
        }
#endif

        friend auto qHash(const Key& key, size_t seed = 0)
        {
            return qHashMulti(seed, key.m_type, qHash(key.m_pos));
        }

        Type m_type;
        Position m_pos;
    };

public:
#if 0
    Info() : QString() { }
    Info(QChar c) : QString(c) { }
    Info(qsizetype size, QChar c) : QString(size, c) { }
    Info(const char* s) : QString(s) { }
#endif
    Info() = default;

    Info(Type type, const Position& pos, const char* value) : m_key(type, pos), m_value(value) {}

    Info(Type type, const Position& pos, const QString& value) : m_key(type, pos), m_value(value) {}

    ~Info() = default;

    Type type() const { return m_key.m_type; }

    Position pos() const { return m_key.m_pos; }

    QString value() const { return m_value; }

    QString typeName() const { return typeName(m_key.m_type); }

    static QString typeName(Type type);

#if __cplusplus >= 202002L
    friend auto operator<=>(const Info& i1, const Info& i2) { return i1.m_key <=> i2.m_key; }
#else
    friend bool operator<(const Info& i1, const Info& i2) { return i1.m_key < i2.m_key; }

    friend bool operator==(const Info& i1, const Info& i2) { return i1.m_key == i2.m_key; }
#endif

    friend size_t qHash(const Info& key, size_t seed = 0) { return qHash(key.m_key, seed); }

private:
    Key m_key;
    QString m_value;

    static const QString s_informationName;
    static const QHash<Type, QString> s_typeNames;
};

/* ========================================================================== */

using InfoList = TreeSet<Info>;

} // namespace lzl::a1

#if 0
    #include <set>
namespace avs8086 {
using InfoList = std::set<Info>;
} // namespace avs8086
#endif

#endif // INFOS_H

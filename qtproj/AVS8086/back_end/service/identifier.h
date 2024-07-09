#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QSet>
#include <QMap>

namespace avs8086 {

class Identifier
{
public:
    enum Type {
        ILLEGAL = 0,
        SEGMENT,
        LABEL,
        VAR,
        FAR,
        NEAR,
    };

public:
    Identifier(const QString& id = "") : Identifier(id, 0, 0, "") { }

    Identifier(
        const QString& id,
        Type type,
        int offset,
        const QString& segment
    );

    Identifier(
        const QString& id,
        int size,
        int offset,
        const QString& segment
    );

    ~Identifier() = default;

    QString id() const { return m_id; }

    Type type() const { return m_type; }

    int size() const { return m_size; }

    int offset() const { return m_offset; }

    QString segment() const { return m_segment; }

    QString typeName() const { return typeName(m_type); }

    static QString typeName(Type type);

    friend auto operator==(const Identifier& i1, const Identifier& i2)
    { return i1.m_id == i2.m_id; }

    friend auto operator<=>(const Identifier& i1, const Identifier& i2)
    { return i1.m_id.toStdString() <=> i2.m_id.toStdString(); }

    friend size_t qHash(const Identifier& key, size_t seed = 0)
    { return qHash(key.m_id, seed); }

private:
    QString m_id;
    Type m_type;
    int m_size;
    int m_offset;
    QString m_segment;

    static const QHash<Type, int> sm_typeSize;
    static const QHash<int, Type> sm_sizeType;
    static const QHash<Type, QString> sm_typeNames;
};

using IdTable = QSet<Identifier>;

#if 0
class IdentifierList
{
public:
    IdentifierList() = default;
    ~IdentifierList() = default;

    bool contains(const QString& id) const;

    bool append(const Identifier& identifier);

    QList<Identifier> list() const;

private:
    QSet<QString> m_set;
    QList<Identifier> m_list;
};
#endif

} // namespace avs8086::ast

#endif // IDENTIFIER_H

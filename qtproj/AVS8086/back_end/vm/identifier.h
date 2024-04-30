#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include <QSet>
#include <QMap>

namespace avs8086::ast {

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
    Identifier();
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

#if 0
    bool operator==(const Identifier& other) const;
#endif

    QString id() const;
    Type type() const;
    int size() const;
    int offset() const;
    QString segment() const;

    QString typeName() const;
    static QString typeName(Type type);

private:
    QString m_id;
    Type m_type;
    int m_size;
    int m_offset;
    QString m_segment;

    static const QMap<Type, int> sm_typeSize;
    static const QMap<int, Type> sm_sizeType;
    static const QMap<Type, QString> sm_typeNames;
};

#if 0
inline uint qHash(const Identifier& key, uint seed = 0)
{
    return ::qHash(key.id(), seed);
}
#endif

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

} // namespace avs8086::ast

#endif // IDENTIFIER_H

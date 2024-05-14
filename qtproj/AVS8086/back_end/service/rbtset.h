#ifndef RBTSET_H
#define RBTSET_H

#include <QMap>

template <class T>
class RBTSet
{
    struct DummyValue {
#if 0
        friend auto operator<=>(const DummyValue&, const DummyValue&) = default;
#endif
    };

    using Map = QMap<T, DummyValue>;

public:
    RBTSet() = default;

    RBTSet(std::initializer_list<T> list)
    {
        for (auto& p : list)
        {
            insert(p);
        }
    }

    ~RBTSet() = default;

    void clear() { m_map.clear(); }

    void swap(RBTSet& other) { this->m_map.swap(other.m_map); }

    qsizetype remove(const T& value) { return m_map.remove(value); }

public:
    qsizetype size() const { return m_map.size(); }

    qsizetype count() const { return m_map.size(); }

    qsizetype length() const { return m_map.size(); }

    bool empty() const { return m_map.empty(); }

    bool isEmpty() const { return m_map.isEmpty(); }

    QList<T> values() const { return m_map.keys(); }

    bool contains(const T& value) const { return m_map.contains(value); }

public:
#if 0
    friend auto operator<=>(const RBTSet&, const RBTSet&) = default;
#else
    friend bool operator!=(const RBTSet& lhs, const RBTSet& rhs)
    { return lhs.m_map != rhs.m_map; }

    friend bool operator==(const RBTSet& lhs, const RBTSet& rhs)
    { return lhs.m_map == rhs.m_map; }
#endif

public:
    class const_iterator;

    class iterator
    {
        friend class RBTSet;
        friend class const_iterator;

        typename Map::iterator i;
        iterator(typename Map::iterator it) : i(it) { }

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = qptrdiff;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;

        iterator() = default;

        const T& operator*() const { return i.key(); }

        const T* operator->() const { return &i.key(); }

        friend bool operator==(const iterator &lhs, const iterator &rhs)
        { return lhs.i == rhs.i; }

        friend bool operator!=(const iterator &lhs, const iterator &rhs)
        { return lhs.i != rhs.i; }

        iterator& operator++() { ++i; return *this; }

        iterator& operator--() { --i; return *this; }

        iterator operator++(int) { auto r = *this; ++i; return r; }

        iterator operator--(int) { auto r = *this; --i; return r; }
    };

    class const_iterator
    {
        friend class RBTSet;

        typename Map::const_iterator i;
        const_iterator(typename Map::const_iterator it) : i(it) { }

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = qptrdiff;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() = default;
        const_iterator(const iterator &o) : i(o.i) { }

        const T& operator*() const { return i.key(); }

        const T* operator->() const { return &i.key(); }

        friend bool
        operator==(const const_iterator& lhs, const const_iterator& rhs)
        { return lhs.i == rhs.i; }

        friend bool
        operator!=(const const_iterator& lhs, const const_iterator& rhs)
        { return lhs.i != rhs.i; }

        const_iterator &operator++() { ++i; return *this; }

        const_iterator &operator--() { --i; return *this; }

        const_iterator operator++(int) { auto r = *this; ++i; return r; }

        const_iterator operator--(int) { auto r = *this; --i; return r; }
    };

    iterator begin() { return m_map.begin(); }

    const_iterator begin() const { return m_map.begin(); }

    const_iterator cbegin() const { return m_map.begin(); }

    const_iterator constBegin() const { return m_map.constBegin(); }

    iterator end() { return m_map.end(); }

    const_iterator end() const { return m_map.end(); }

    const_iterator cend() const { return m_map.end(); }

    const_iterator constEnd() const { return m_map.constEnd(); }

    iterator erase(const_iterator it)
    { Q_ASSERT(it != constEnd()); return m_map.erase(it.i); }

    iterator insert(const T& value)
    { return m_map.insert(value, DummyValue()); }

    iterator insert(T&& value)
    { return m_map.insert(std::move(value), DummyValue()); }

    iterator insert(const_iterator pos, const T& value)
    { return m_map.insert(pos.i, value, DummyValue()); }

    void insert(const RBTSet<T>& o) { m_map.insert(o.m_map); }

    iterator find(const T& value) { return m_map.find(value); }

    const_iterator find(const T& value) const { return m_map.find(value); }

    const_iterator constFind(const T& value) const { return find(value); }

    iterator lowerBound(const T& value) { return m_map.lowerBound(value); }

    iterator upperBound(const T& value) { return m_map.upperBound(value); }

    const_iterator lowerBound(const T& value) const
    { return m_map.lowerBound(value); }

    const_iterator upperBound(const T& value) const
    { return m_map.upperBound(value); }

private:
    Map m_map;
};

#endif // RBTSET_H

#ifndef INCLUDE_LA1_LIB_CONFIG_H
#define INCLUDE_LA1_LIB_CONFIG_H

#include <QHash>
#include <QMap>
#include <QSet>

namespace lzl {

template <typename Key, typename T>
using HashMap = QMap<Key, T>;

template <typename Key, typename T>
using TreeMap = QMap<Key, T>;

template <typename T>
using HashSet = QSet<T>;

template <typename T>
class TreeSet
{
    struct DummyValue
    {
        bool operator==(const DummyValue&) const noexcept { return true; }
    };

    using Map = QMap<T, DummyValue>;

public:
    using difference_type = qptrdiff;
    using size_type = qsizetype;

    TreeSet() = default;

    TreeSet(std::initializer_list<T> list)
    {
        for (auto& p : list)
        {
            insert(p);
        }
    }

    ~TreeSet() = default;

    friend bool operator!=(const TreeSet& lhs, const TreeSet& rhs) { return lhs.q_map != rhs.q_map; }
    friend bool operator==(const TreeSet& lhs, const TreeSet& rhs) { return lhs.q_map == rhs.q_map; }

    void swap(TreeSet& other) noexcept { q_map.swap(other.q_map); }

    size_type size() const { return q_map.size(); }
    size_type count() const { return size(); }
    size_type count(const T& value) const { return q_map.count(value); }

    bool empty() const { return q_map.empty(); }
    bool isEmpty() const { return q_map.isEmpty(); }

    void detach() { q_map.detach(); }
    bool isDetached() const { return q_map.isDetached(); }

    void clear() { q_map.clear(); }

    bool remove(const T& value) { return q_map.remove(value) != 0; }

    template <typename Predicate>
    size_type removeIf(Predicate predicate)
    {
        return q_map.removeIf(predicate);
    }

    bool contains(const T& value) const { return q_map.contains(value); }

    bool contains(const TreeSet& set) const;

    QList<T> values() const { return q_map.keys(); }

    inline T& first() { return q_map.first(); }
    inline const T& first() const { return q_map.first(); }
    inline T& last() { return q_map.end(); }
    inline const T& last() const { return q_map.end(); }

    /* Iterator */

    class const_iterator;

    class iterator
    {
        friend class TreeSet;
        friend class const_iterator;
        typename Map::iterator i;
        iterator(typename Map::iterator it) : i(it) {}

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = qptrdiff;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;

        iterator() = default;

        const T& operator*() const { return i.key(); }
        const T* operator->() const { return &i.key(); }
        friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs.i == rhs.i; }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) { return lhs.i != rhs.i; }

        iterator& operator++()
        {
            ++i;
            return *this;
        }
        iterator& operator--()
        {
            --i;
            return *this;
        }
        iterator operator++(int)
        {
            auto r = *this;
            ++i;
            return r;
        }
        iterator operator--(int)
        {
            auto r = *this;
            --i;
            return r;
        }
    };

    class const_iterator
    {
        friend class TreeSet;
        typename Map::const_iterator i;
        const_iterator(typename Map::const_iterator it) : i(it) {}

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type = qptrdiff;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() = default;
        const_iterator(const iterator& o) : i(o.i) {}

        const T& operator*() const { return i.key(); }
        const T* operator->() const { return &i.key(); }
        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs)
        {
            return lhs.i == rhs.i;
        }
        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs)
        {
            return lhs.i != rhs.i;
        }

        const_iterator& operator++()
        {
            ++i;
            return *this;
        }
        const_iterator& operator--()
        {
            --i;
            return *this;
        }
        const_iterator operator++(int)
        {
            auto r = *this;
            ++i;
            return r;
        }
        const_iterator operator--(int)
        {
            auto r = *this;
            --i;
            return r;
        }
    };

    // STL style
    iterator begin() { return q_map.begin(); }
    const_iterator begin() const noexcept { return q_map.begin(); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator constBegin() const noexcept { return begin(); }
    iterator end() { return q_map.end(); }
    const_iterator end() const noexcept { return q_map.end(); }
    const_iterator cend() const noexcept { return cbegin(); }
    const_iterator constEnd() const noexcept { return begin(); }

    iterator erase(const_iterator it) { return q_map.erase(it.i); }

    iterator erase(const_iterator afirst, const_iterator alast) { return q_map.erase(afirst.i, alast.i); }

    // more Qt
    typedef iterator Iterator;
    typedef const_iterator ConstIterator;

    iterator find(const T& value) { return q_map.find(value); }
    const_iterator find(const T& value) const { return q_map.find(value); }
    const_iterator constFind(const T& value) const { return find(value); }
    iterator lowerBound(const T& value) { return q_map.lowerBound(value); }
    const_iterator lowerBound(const T& value) const { return q_map.lowerBound(value); }
    iterator upperBound(const T& value) { return q_map.upperBound(value); }
    const_iterator upperBound(const T& value) const { return q_map.upperBound(value); }
    iterator insert(const T& value) { q_map.insert(value, DummyValue()); }
    iterator insert(T&& value) { return q_map.insert(std::move(value), DummyValue()); }
    iterator insert(const_iterator pos, const T& value) { return q_map.insert(pos.i, value, DummyValue()); }
    void insert(const TreeSet& set) { q_map.insert(set.q_map); }
    TreeSet& unite(const TreeSet& other);
    TreeSet& intersect(const TreeSet& other);
    bool intersects(const TreeSet& other) const;
    TreeSet& subtract(const TreeSet& other);

    QPair<iterator, iterator> equal_range(const T& value) { return q_map.equal_range(value); }

    QPair<const_iterator, const_iterator> equal_range(const T& value) const
    {
        return q_map.equal_range(value);
    }

    // comfort
    TreeSet& operator<<(const T& value)
    {
        insert(value);
        return *this;
    }

    TreeSet& operator<<(const TreeSet& set)
    {
        insert(set);
        return *this;
    }

    TreeSet& operator|=(const TreeSet& other) { return unite(other); }
    TreeSet& operator|=(const T& other) { return insert(other); }
    TreeSet& operator&=(const TreeSet& other) { return intersect(other); }
    TreeSet& operator&=(const T& other)
    {
        TreeSet<T> result;
        if (contains(other))
        {
            result.insert(other);
        }
        return (*this = result);
    }
    TreeSet& operator+=(const TreeSet& other) { return unite(other); }
    TreeSet& operator+=(const T& other) { return insert(other); }
    TreeSet& operator-=(const TreeSet& other) { return subtract(other); }
    TreeSet& operator-=(const T& other) { return remove(other); }
    TreeSet& operator|(const TreeSet& other) const
    {
        TreeSet<T> result = *this;
        result |= other;
        return result;
    }
    TreeSet& operator&(const TreeSet& other) const
    {
        TreeSet<T> result = *this;
        result &= other;
        return result;
    }
    TreeSet& operator+(const TreeSet& other) const
    {
        TreeSet<T> result = *this;
        result += other;
        return result;
    }
    TreeSet& operator-(const TreeSet& other) const
    {
        TreeSet<T> result = *this;
        result -= other;
        return result;
    }

private:
    Map q_map;
};

template <typename T>
inline bool TreeSet<T>::contains(const TreeSet& set) const
{
    for (auto i = set.constBegin(); i != set.constEnd(); ++i)
    {
        if (!contains(*i))
        {
            return false;
        }
    }
    return true;
}

template <typename T>
inline TreeSet<T>& TreeSet<T>::unite(const TreeSet& other)
{
    if (!q_map.isSharedWith(other.q_map))
    {
        for (const T& e : other)
        {
            insert(e);
        }
    }
    return *this;
}

template <typename T>
inline TreeSet<T>& TreeSet<T>::intersect(const TreeSet& other)
{
    TreeSet<T> copy1;
    TreeSet<T> copy2;
    if (size() <= other.size())
    {
        copy1 = *this;
        copy2 = other;
    }
    else
    {
        copy1 = other;
        copy2 = *this;
        *this = copy1;
    }
    for (const auto& e : copy1)
    {
        if (!copy2.contains(e))
        {
            remove(e);
        }
    }
    return *this;
}

template <typename T>
inline bool TreeSet<T>::intersects(const TreeSet& other) const
{
    const bool otherIsBigger = other.size() > size();
    const TreeSet& smallestSet = otherIsBigger ? *this : other;
    const TreeSet& biggestSet = otherIsBigger ? other : *this;
    auto i = smallestSet.cbegin();
    auto e = smallestSet.cend();

    while (i != e)
    {
        if (biggestSet.contains(*i))
        {
            return true;
        }
        ++i;
    }

    return false;
}

template <typename T>
inline TreeSet<T>& TreeSet<T>::subtract(const TreeSet& other)
{
    if (q_map.isSharedWith(other.q_map))
    {
        clear();
    }
    else
    {
        for (const auto& e : other)
        {
            remove(e);
        }
    }
    return *this;
}


} // namespace lzl

#endif // INCLUDE_LA1_LIB_CONFIG_H

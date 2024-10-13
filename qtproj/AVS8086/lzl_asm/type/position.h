#ifndef POSITION_H
#define POSITION_H

#include <QSet>
#include <QMap>

namespace avs8086 {

class Position
{
public:
    struct Value {
#if 0
        friend bool operator<(const Value& v1, const Value& v2)
        {
            if (v1.row != v2.row)
                return v1.row < v2.row;
            if (v1.column != v2.column)
                return v1.column < v2.column;
            return v1.length < v2.length;
        }

        friend bool operator==(const Value& v1, const Value& v2)
        {
            return v1.row == v2.row
                   && v1.column == v2.column
                   && v1.length == v2.length;
        }
#endif
        friend auto operator<=>(const Value&, const Value&) = default;
        /* QPlainTextEdit 的行列号返回 int, 所以这里存储用 int */
        int row = 0;
        int column = 0;
        int length = 0;
    };

public:
    Position() = default;

    Position(Value value) : m_value(value) { }

    Position(int row, int column, int length)
        : m_value({row, column, length})
    { }

#if 0
    Position(const Position& left, const Position& right)
        : m_value({
            (Q_ASSERT(left.row() == right.row()), left.row()),
            (Q_ASSERT(left.column() < right.column()), left.column()),
            right.endColumn() - left.column()
        })
    { }
#endif

    ~Position() = default;

    int row() const { return m_value.row; }

    int column() const { return m_value.column; }

    int length() const { return m_value.length; }

    int endColumn() const { return m_value.column + m_value.length; }

    Value value() const { return m_value; }

    friend Position operator+(const Position& left, const Position& right)
    {
        Q_ASSERT(left.m_value.row == right.m_value.row);
        Q_ASSERT(left.m_value.column < right.m_value.column);
        return {left.row(), left.column(), right.endColumn() - left.column()};
    }

#if 0
#if 0
    bool operator<(const Position& o) const { return m_value < o.m_value; }

    bool operator==(const Position& o) const { return m_value == o.m_value; }
#endif
    friend bool operator<(const Position& p1, const Position& p2)
    {
        if (p1.m_value.row != p2.m_value.row)
            return p1.m_value.row < p2.m_value.row;
        if (p1.m_value.column != p2.m_value.column)
            return p1.m_value.column < p2.m_value.column;
        return p1.m_value.length < p2.m_value.length;
    }

    friend bool operator==(const Position& p1, const Position& p2)
    {
        return p1.m_value.row == p2.m_value.row
               && p1.m_value.column == p2.m_value.column
               && p1.m_value.length == p2.m_value.length;
    }
#endif

    friend auto operator<=>(const Position&, const Position&) = default;

    friend auto qHash(const Position& key, size_t seed = 0)
    {
        return qHashMulti(
            seed, key.m_value.row, key.m_value.column, key.m_value.length
        );
    }

private:
    Value m_value;
};

} // namespace avs8086

#endif // POSITION_H

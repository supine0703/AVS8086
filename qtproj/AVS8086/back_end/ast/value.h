#ifndef VALUE_H
#define VALUE_H

#include <QByteArray>
#include <QString>

namespace avs8086::ast {

// 感觉封装的很...的一个值类(计算为无符号类型计算)
class Value
{
public:
    Value(int v = 0);
    Value(const char* v);
    Value(const std::string& v);
    Value(const QString& v);
    Value(const QByteArray& v);
    Value(const Value& other);
    ~Value();

    Value& operator=(const Value& other);

    int toInt() const;
    QString toHex(char c = 0) const;
    QString toString() const;

    QByteArray value() const;

    bool isExtend() const;
    void setExtend(bool yes);

    Value operator~() const;
    Value operator+() const;
    Value operator-() const;

    Value operator+(const Value& other) const;
    Value operator-(const Value& other) const;
    Value operator*(const Value& other) const;
    Value operator/(const Value& other) const;
    Value operator %(const Value& other) const;

    Value operator&(const Value& other) const;
    Value operator^(const Value& other) const;
    Value operator|(const Value& other) const;
    Value operator<<(const Value& other) const;
    Value operator>>(const Value& other) const;

    Value& operator+=(const Value& other);
    Value& operator-=(const Value& other);
    Value& operator*=(const Value& other);
    Value& operator/=(const Value& other);
    Value& operator%=(const Value& other);

    Value& operator&=(const Value& other);
    Value& operator^=(const Value& other);
    Value& operator|=(const Value& other);
    Value& operator<<=(const Value& other);
    Value& operator>>=(const Value& other);

    bool operator>(const Value& other) const;
    bool operator>=(const Value& other) const;
    bool operator<(const Value& other) const;
    bool operator<=(const Value& other) const;
    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;

private:
    static int init_nb(
        QByteArray& nb1, QByteArray& nb2,
        const QByteArray& b1, const QByteArray& b2
    );

    static void least(QByteArray& b);

private:
    bool m_extend = true;
    QByteArray m_value;
};

} // namespace avs8086::ast

#endif // VALUE_H

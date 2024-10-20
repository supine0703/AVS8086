#ifndef LA1_GUI_CONVERT_QVARIANT_H
#define LA1_GUI_CONVERT_QVARIANT_H

#include <QVariant>

namespace la1::gui {

template <typename T>
struct ConvertQVariant;

// 针对不同类型特化
template <>
struct ConvertQVariant<int>
{
    static int convert(const QVariant& value) { return value.toInt(); }
};

template <>
struct ConvertQVariant<bool>
{
    static bool convert(const QVariant& value) { return value.toBool(); }
};

template <>
struct ConvertQVariant<double>
{
    static double convert(const QVariant& value) { return value.toDouble(); }
};

template <>
struct ConvertQVariant<QString>
{
    static QString convert(const QVariant& value) { return value.toString(); }
};

} // namespace la1::gui

#endif // LA1_GUI_CONVERT_QVARIANT_H

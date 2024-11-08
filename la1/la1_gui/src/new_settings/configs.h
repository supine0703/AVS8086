/**
 *  AVS8086 - See <https://github.com/supine0703/AVS8086> to know more.
 *
 *  Copyright (C) <2024>  <李宗霖>  github <https://github.com/supine0703>.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef LA1_GUI_CONFIGS_H
#define LA1_GUI_CONFIGS_H

#include "convert_qvariant.h"

#include <QSettings>

namespace la1::gui {


// function_traits模板
template <typename T>
struct function_traits;

template <typename R, typename... Args>
struct function_traits<std::function<R(Args...)>>
{
    using args_type = std::tuple<Args...>;
};

// lambda_traits模板
template <typename F> // 使用递归可以包解 std::function<R(Args...)>([](Args...){ ... })
struct lambda_traits : public lambda_traits<decltype(&F::operator())>
{
};

template <typename C, typename R, typename... Args>
struct lambda_traits<R (C::*)(Args...) const>
{
    using args_type = std::tuple<Args...>;
};

// 辅助类型，检测F是否是Lambda表达式
template <typename T>
struct is_lambda : std::false_type
{
};

template <typename C, typename R, typename... Args>
struct is_lambda<R (C::*)(Args...) const> : std::true_type
{
};

// 定义 Lambda 概念，用于检查 Lambda 表达式并提取参数类型
template <typename T>
concept IsLambda = requires(T t) {
    { &T::operator() };
};

// 提取 Lambda 参数类型的模板
template <typename F>
struct LambdaTraits;

template <typename C, typename R, typename... Args>
struct LambdaTraits<R (C::*)(Args...) const>
{
    using args_type = std::tuple<Args...>;
};


class Configs : public QObject
{
    Configs(const Configs&) = delete;
    Configs(Configs&&) = delete;
    Configs& operator=(const Configs&) = delete;
    Configs& operator=(Configs&&) = delete;

    Q_OBJECT

public:
    static void setValue(QAnyStringView key, const QVariant& value);

    static QVariant value(QAnyStringView key);

    template <typename Arg>
    static void registerSignal(const QString& key, std::function<void(Arg)> emitFn)
    {
        using ArgType = Arg;
        Q_ASSERT(!instance().m_map.contains(key));
        instance().m_map.insert(key, [emitFn](const QVariant& value) {
            emitFn(ConvertQVariant<ArgType>::convert(value));
        });
    }

    // registerSignal 函数模板，只接受 Lambda 表达式
    // template <IsLambda F>
    // static void registerSignal(const QString& key, F&& emitFn)
    // {
    //     using args_type = typename LambdaTraits<decltype(&F::operator())>::args_type;
    //     static_assert(std::tuple_size<args_type>::value == 1, "Lambda must take exactly one argument.");
    //     using ArgType = typename std::tuple_element<0, args_type>::type;
    //     Q_ASSERT(!instance().m_map.contains(key));
    //     instance().m_map.insert(key, [emitFn](const QVariant& value) {
    //         emitFn(ConvertQVariant<ArgType>::convert(value));
    //     });
    // }

    template <typename Obj, typename MemFn>
    static void registerSignal(const QString& key, Obj* obj, MemFn fn)
    {
        registerSignal(key, [obj, fn] (auto arg) {
            (obj->*fn)(arg);
        });
    }

    // template <typename Obj, typename Arg>
    // static void registerSignal(const QString& key, Obj* obj, void (Obj::*memFn)(Arg))
    // {
    //     using ArgType = Arg;
    //     Q_ASSERT(!instance().m_map.contains(key));
    //     instance().m_map.insert(key, [=](const QVariant& value) {
    //         (obj->*memFn)(ConvertQVariant<ArgType>::convert(value));
    //     });
    // }

    static void run()
    {
        for (const auto& a : qAsConst(instance().m_map))
        {
            a(QVariant(999));
        }
    }


private:
    static Configs& instance();

private:
    Configs() : m_settings(CONFIG_INI, QSettings::IniFormat) {}
    QSettings m_settings;
    QMap<QString, std::function<void(const QVariant&)>> m_map;

signals:
    void configChanged(QAnyStringView, const QVariant&);
};


} // namespace la1::gui


#endif // LA1_GUI_CONFIGS_H

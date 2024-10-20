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

    // function_traits模板
    template <typename T>
    struct function_traits;

    template <typename R, typename... Args>
    struct function_traits<std::function<R(Args...)>>
    {
        using args_type = std::tuple<Args...>;
    };

    // lambda_traits模板
    template <typename F>
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

    // registerSignal函数模板，只接受Lambda表达式
    template <typename F>
    // static void registerSignal(const QString& key, F&& emitFn)
    static auto registerSignal(const QString& key, F&& emitFn) ->
        typename std::enable_if<is_lambda<decltype(&F::operator())>::value>::type
    { // auto 启用条件编译。通过std::enable_if<is_lambda<...>>来判断模板是否实例化
        using args_type = typename lambda_traits<F>::args_type;
        static_assert(
            std::tuple_size<args_type>::value == 1, "Lambda must take exactly one argument."
        );
        using ArgType = typename std::tuple_element<0, args_type>::type;
        Q_ASSERT(!instance().m_map.contains(key));
        instance().m_map.insert(key, [emitFn](const QVariant& value) {
            emitFn(ConvertQVariant<ArgType>::convert(value));
        });
    }

    template <typename OBJ, typename Arg>
    static void registerSignal(const QString& key, OBJ* obj, void (OBJ::*method)(Arg))
    {
        using ArgType = Arg;
        Q_ASSERT(!instance().m_map.contains(key));
        instance().m_map.insert(key, [=](const QVariant& value) {
            (obj->*method)(ConvertQVariant<ArgType>::convert(value));
        });
    }

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

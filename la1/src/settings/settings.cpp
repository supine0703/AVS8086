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


#include "settings.h"

QSettings& Settings::getSettings()
{
    static QSettings s(CONFIG_INI, QSettings::IniFormat);
    return s;
}

QSettings& Settings::getUISettings()
{
    static QSettings s(CONFIG_UI_INI, QSettings::IniFormat);
    return s;
}

QSettings& Settings::getSettings(const QString& file)
{
    static QSettings s;
    s.setPath(QSettings::IniFormat, QSettings::UserScope, file);
    return s;
}

void Settings::checkSettings(QAnyStringView key, int defaultValue, int min, int max)
{
    auto& s(Settings::getSettings());
    if (s.contains(key))
    {
        bool ok;
        int n = s.value(key).toInt(&ok);
        if (ok && (min <= n && n <= max))
        {
            return;
        }
    }
    s.setValue(key, defaultValue);
}

void Settings::checkSettings(
    QAnyStringView key, const QString& defaultValue, const QStringList& values, const char* split
    )
{
    auto& s(Settings::getSettings());
    if (s.contains(key))
    {
        auto str = s.value(key).toString();
        if (!checkSettings(split == nullptr ? (QStringList() << str) : str.split(split), values))
        {
            s.setValue(key, defaultValue);
        }
    }
}

bool Settings::checkSettings(const QStringList& values, const QStringList& allValues)
{
    return std::all_of(values.begin(), values.end(), [&allValues](const QString& v) {
        return allValues.contains(v.trimmed());
    });
}

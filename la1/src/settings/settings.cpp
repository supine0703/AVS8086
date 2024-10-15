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

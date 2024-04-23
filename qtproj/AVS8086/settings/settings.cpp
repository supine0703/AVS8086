#include "settings.h"

QSettings& Settings::getSettings()
{
    static QSettings s(CONFIG_INI, QSettings::IniFormat);
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
            return;
    }
    s.setValue(key, defaultValue);
}

void Settings::checkSettings(QAnyStringView key, QString defaultValue, QStringList values)
{
    auto& s(Settings::getSettings());
    if (s.contains(key))
    {
        if (!values.isEmpty())
        {
            QString value = s.value(key).toString();
            for (const auto& v : values)
                if (value == v)
                    return;
        }
        else
            return;
    }
    s.setValue(key, defaultValue);
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


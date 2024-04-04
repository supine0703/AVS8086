#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
    Settings() = delete;
    Settings(const Settings&) = delete;
    Settings(Settings&&) = delete;
    Settings& operator=(const Settings&) = delete;
    Settings& operator=(Settings&&) = delete;

public:
    static QSettings& getSettings();

    static void checkSettings(
        QAnyStringView key, int defaultValue, int min = INT32_MIN, int max = INT32_MAX
    );
    static void checkSettings(
        QAnyStringView key, QString defaultValue, QStringList values = {}
    );
};

#define SETTINGS Settings::getSettings
#define CHECK_SETTINGS Settings::checkSettings


#define _APP_SCALE_      "/app/scale"

#define _TEXT_EDIT_FILE_ "/text_edit/file"


#endif // SETTINGS_H

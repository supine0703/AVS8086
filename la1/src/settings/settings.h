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

    static QSettings& getUISettings();

    static QSettings& getSettings(const QString& file);

    static void checkSettings(
        QAnyStringView key, int defaultValue, int min = INT32_MIN, int max = INT32_MAX
    );

    static void checkSettings(
        QAnyStringView key,
        const QString& defaultValue,
        const QStringList& values,
        const char* split = nullptr
    );

private:
    static bool checkSettings(const QStringList& values, const QStringList& allValues);
};

#define SETTINGS       Settings::getSettings
#define CHECK_SETTINGS Settings::checkSettings

#define _APP_SCALE_ "/app/scale"
#define _APP_FONTS_ "/app/fonts"

#define _TEXT_EDIT_FILE_ "/text_edit/file"


#define SETTINGS_UI Settings::getUISettings

#define _WINDOW_SIZE_ "/window/size"
#define _WINDOW_POS_  "/window/pos"

#define _NEW_DEFAULT_PATH "/new/default_path"

#endif // SETTINGS_H

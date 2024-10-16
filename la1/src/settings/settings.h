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

#define _APP_OPEN_FILE_ "/app/open_file"
#define _APP_CLC_1_ "/app/code_line_color_1"
#define _APP_CLC_2_ "/app/code_line_color_2"


#define SETTINGS_UI Settings::getUISettings

#define _WINDOW_SIZE_ "/window/size"
#define _WINDOW_POS_  "/window/pos"

#define _NEW_DEFAULT_PATH "/new/default_path"

#endif // SETTINGS_H

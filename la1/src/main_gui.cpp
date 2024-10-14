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


#include "./widget/mainwidget.h"
#include "./settings/settings.h"

#include <QApplication>
#include <QFontDatabase>


void appInit(); // 必须在 QApplication 创建之后使用

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    appInit();

    MainWidget w;
    w.show();

    return a.exec();
}


/* ========================================================================= */

// 初始化应用程序的一些设置
inline void appInit()
{
    Q_ASSERT(QCoreApplication::instance() != nullptr);

    // 设置字体
    auto getFontName = [](QStringList res) -> QStringList {
        QStringList s;
        for (const auto& r : res)
        {
            s << QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(r)).at(0);
        }
        return s;
    };
    auto fontName = getFontName({
        ":/font/iconfont/iconfont.ttf",
        ":/font/d_font/JetBrainsMono-Medium.ttf",
        ":/font/d_font/JetBrainsMono-MediumItalic.ttf",
        ":/font/d_font/YouSheShaYuFeiTeJianKangTi-2.ttf",
    });

    CHECK_SETTINGS(
        _APP_SCALE_, "1",
        { "0.5", "0.75", "1", "1.25", "1.5", "1.75", "2", "2.25", "2.5" }
    );
    int fontSize = 8 * SETTINGS().value(_APP_SCALE_).toDouble();

    QApplication::setFont(QFont(fontName, fontSize));

    // ...
}

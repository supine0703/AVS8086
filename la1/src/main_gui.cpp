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


#include "./settings/settings.h"
#include "./widget/mainwidget.h"

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
    auto getFontList = [](QStringList res) -> QStringList {
        QStringList fs;
        for (const auto& r : res)
        {
            auto f =
                QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(r)).at(0);
            if (!fs.contains(f))
            {
                fs << f;
            }
        }
        return fs;
    };
    auto fontList = getFontList({
        ":/font/iconfont/iconfont.ttf",
        ":/font/d_font/Space.ttf",
        ":/font/d_font/JetBrainsMono[wght].ttf",
        ":/font/d_font/JetBrainsMono-Italic[wght].ttf",
        ":/font/d_font/YouSheShaYuFeiTeJianKangTi.ttf",
    });
    fontList.removeAll("iconfont"); // 不应该作为通用字体 会导致空白行和文字行高度不一致
    fontList.removeAll("Space"); // Space: 在 0x1fdc 创建了一个空格 行间距 `200`
                                 // 主要用作抵消 Qt因不同字体差异带来的不良影响

    // 检测字体是否有效 没效则用默认字体
    CHECK_SETTINGS(_APP_FONTS_, fontList.join(","), QFontDatabase::families(), ",");
    QString family("Space,");
    family.append(SETTINGS().value(_APP_FONTS_).toString());

    // 设置字体倍率 但是这似乎不是一个好的方案
    CHECK_SETTINGS(
        _APP_SCALE_, "1", { "0.5", "0.75", "1", "1.25", "1.5", "1.75", "2", "2.25", "2.5" }
    );
    int fontSize = 8 * SETTINGS().value(_APP_SCALE_).toDouble();

    QApplication::setFont(QFont(family, fontSize));
}

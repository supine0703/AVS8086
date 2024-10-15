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
    auto getFontName = [](QStringList res) -> QStringList {
        QStringList s;
        for (const auto& r : res)
        {
            s << QFontDatabase::applicationFontFamilies(QFontDatabase::addApplicationFont(r)).at(0);
        }
        qDebug() << s;
        s.removeDuplicates();
        return s;
    };
    auto fontName = getFontName({
        ":/font/d_font/JetBrainsMono-Space.ttf",
        ":/font/iconfont/iconfont.ttf",
        ":/font/d_font/JetBrainsMono[wght].ttf",
        ":/font/d_font/JetBrainsMono-Italic[wght].ttf",
        ":/font/d_font/YouSheShaYuFeiTeJianKangTi.ttf",
    });
    fontName.removeAll("iconfont"); // 不应该作为通用字体 会导致空白行和文字行高度不一致

    // 检测字体是否有效 没效则用默认字体
    CHECK_SETTINGS(_APP_FONTS_, fontName.join(","), QFontDatabase::families(), ",");
    QString family = SETTINGS().value(_APP_FONTS_).toString();

    // 设置字体倍率 但是这似乎不是一个好的方案
    CHECK_SETTINGS(
        _APP_SCALE_, "1", { "0.5", "0.75", "1", "1.25", "1.5", "1.75", "2", "2.25", "2.5" }
    );
    int fontSize = 8 * SETTINGS().value(_APP_SCALE_).toDouble();

    auto font = QFont(family, fontSize);
    // font.setItalic(true);
    QApplication::setFont(font);

    auto f1 = QFont(family[1], fontSize);
    auto f2 = QFont(family[2], fontSize);
    qDebug() << family;

    QFontMetrics fm0(QFont(family[0], fontSize));
    QFontMetrics fm1(f1);
    QFontMetrics fm2(f2);
    QFontMetrics fm3(font);

    qDebug() << fm0.height() << fm0.xHeight() << fm0.capHeight();
    qDebug() << fm1.height() << fm1.xHeight() << fm1.capHeight();
    qDebug() << fm2.height() << fm2.xHeight() << fm2.capHeight();
    qDebug() << fm3.height() << fm3.xHeight() << fm3.capHeight();

    // ...
}

// #include "widget.h"
#include "texteditwidget.h"
#include "settings.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    auto getFontName = [](QStringList sL) {
        QString f(sL.at(0));
        for (int i = 1, end = sL.length(); i < end; i++) {
            f = QString("%1,%2").arg(
                f,
                QFontDatabase::applicationFontFamilies(
                    QFontDatabase::addApplicationFont(sL.at(i))
                ).at(0)
            );
        }
        return f;
    };
    QString fontName = getFontName({
        ":/font/iconfont/iconfont.ttf",
        ":/font/d_font/JetBrainsMonoNL-Medium.ttf",
        ":/font/d_font/JetBrainsMonoNL-MediumItalic.ttf",
        ":/font/d_font/YouSheShaYuFeiTeJianKangTi-2.ttf",
    });

    CHECK_SETTINGS(
        _APP_SCALE_, "1",
        { "0.5", "0.75", "1", "1.25", "1.5", "1.75", "2", "2.25", "2.5" }
    );
    int fontSize = 8 * SETTINGS().value(_APP_SCALE_).toDouble();

    QApplication::setFont(QFont(fontName, fontSize));
    QApplication::setWindowIcon(QIcon(":/ico/img/AppIcon.ico"));

    TextEditWidget w;
    w.show();

    return a.exec();
}

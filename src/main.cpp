#include "dialogs/engine_settings_dialog.h"
#include "mainwindow.h"
#include "utils/logger.h"

#include <QApplication>
#include <QLocale>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);
    QApplication a(argc, argv);

    QFont defaultFont = QApplication::font();
    if (defaultFont.pointSize() > 0) {
        defaultFont.setPointSize(defaultFont.pointSize() + 2);
    } else if (defaultFont.pixelSize() > 0) {
        defaultFont.setPixelSize(defaultFont.pixelSize() + 2);
    } else {
        defaultFont.setPointSize(11);
    }
    QApplication::setFont(defaultFont);

    Logger::init("chess_game.log");
    qInfo("System Log is ready work!");

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages)
    {
        QString lang = QLocale(locale).name();       // "vi_VN"
        lang = lang.section('_', 0, 0);              // only need "vi"
        const QString baseName = "chess_" + lang;    // "chess_vi"

        if (translator.load(":/i18n/" + baseName + ".qm"))
        {
            a.installTranslator(&translator);
            break;
        }
    }


    MainWindow w;

    // Load default language from system or QSettings
    QString langCode = QLocale::system().name(); // example: "en_US"
    QSettings settings("YourCompany", "YourApp");
    langCode = settings.value("language", langCode).toString();

    w.loadLanguage(langCode);

    // Use embedded resources for icons
    w.setWindowIcon(QIcon(":/app_icon.ico"));
    w.show();
    return a.exec();
}

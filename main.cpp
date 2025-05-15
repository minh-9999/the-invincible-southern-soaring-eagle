#include "mainwindow.h"

#include <QApplication>
#include <QLocale>


int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);
    QApplication a(argc, argv);

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

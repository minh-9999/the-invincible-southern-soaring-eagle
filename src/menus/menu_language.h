#ifndef MENU_LANGUAGE_H
#define MENU_LANGUAGE_H

#include <QMenu>
#include <QTranslator>
#include <QMainWindow>
#include <QObject>

class LanguageMenu : public QObject
{
    Q_OBJECT

public:
    explicit LanguageMenu(QWidget* parent = nullptr);
    void languageMenus(QMainWindow* window);
    QString currentLanguageCode() const;

signals:
    void languageChanged(const QString& langCode);


private slots:
    void changeLanguage();

private:
    void addLanguageOption(QMenu* menu, const QString& name, const QString& code, bool checked);

    QString currentLangCode;
    QActionGroup* group = nullptr;
};

#endif // MENU_LANGUAGE_H

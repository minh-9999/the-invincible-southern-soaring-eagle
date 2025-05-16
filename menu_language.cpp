#include "menu_language.h"
#include <QActionGroup>
#include <QMenuBar>
#include <QSettings>


LanguageMenu::LanguageMenu(QWidget* parent) : QObject(parent)
{
    group = new QActionGroup(this);
    group->setExclusive(true);
    connect(group, &QActionGroup::triggered, this, &LanguageMenu::changeLanguage);

    // Read the language saved from last time (default is "en")
    QSettings settings("YourCompany", "YourApp");
    currentLangCode = settings.value("language", "en").toString();

    // Make sure currentLangCode has a value
    if (currentLangCode.isEmpty())
        currentLangCode = "en";
}

void LanguageMenu::addLanguageOption(QMenu* menu, const QString& name, const QString& code, bool checked)
{
    QAction* action = new QAction(name, menu);
    action->setCheckable(true);
    action->setChecked(checked);
    action->setData(code);
    action->setActionGroup(group);
    menu->addAction(action);
}

void LanguageMenu::changeLanguage()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) return;

    currentLangCode = action->data().toString();
    // Remember to go to settings
    QSettings settings("YourCompany", "YourApp");
    settings.setValue("language", currentLangCode);

    emit languageChanged(currentLangCode);
}


QString LanguageMenu::currentLanguageCode() const
{
    return currentLangCode;
}

void LanguageMenu::languageMenus(QMainWindow* window)
{
    QMenu *languageMenu = new QMenu(tr("Language"), window);

    addLanguageOption(languageMenu, tr("English (Default)"), "en", true);
    addLanguageOption(languageMenu, tr("Vietnamese"), "vi", false);
    addLanguageOption(languageMenu, tr("Chinese (Simple)"), "zh_CN", false);
    addLanguageOption(languageMenu, tr("Chinese (Traditional)"), "zh_TW", false);
    addLanguageOption(languageMenu, tr("French"), "fr", false);
    addLanguageOption(languageMenu, tr("Portuguese"), "pt", false);
    addLanguageOption(languageMenu, tr("Spanish"), "es", false);
    addLanguageOption(languageMenu, tr("Russian"), "ru", false);

    window->menuBar()->addMenu(languageMenu);

}


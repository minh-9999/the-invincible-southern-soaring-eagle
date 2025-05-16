#include "menu_book.h"
#include <QMenuBar>
#include <QAction>
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>


MenuBook::MenuBook(QObject* parent): QObject(parent)
{
}

void MenuBook::bookMenus(QMainWindow* window)
{
    QMenu *bookMenu = new QMenu(tr("Book"), window);

    QAction *openBookSettingsAct = bookMenu->addAction(
        QIcon(":/icons/book_settings_icon.png"),
        tr("OpenBook Settings")
    );
    openBookSettingsAct->setShortcut(Qt::Key_F10);

    QAction *addMovesToBookAct = bookMenu->addAction(
        QIcon(":/icons/book_add_icon.png"),
        tr("Add Moves To OpenBook")
    );
    addMovesToBookAct->setShortcut(Qt::Key_F11);

    QAction *enableOpenBookAct = bookMenu->addAction(tr("Enable OpenBook"));
    enableOpenBookAct->setCheckable(true);
    enableOpenBookAct->setChecked(true); // default On

    window->menuBar()->addMenu(bookMenu);

    connect(openBookSettingsAct, &QAction::triggered, this, &MenuBook::onOpenBookSettings);
    connect(addMovesToBookAct, &QAction::triggered, this, &MenuBook::onAddMovesToBook);
    connect(enableOpenBookAct, &QAction::toggled, this, &MenuBook::onEnableOpenBookToggled);
}


void MenuBook::onOpenBookSettings()
{
    QMessageBox::information(nullptr, tr("OpenBook Settings"), tr("Here you can configure OpenBook settings."));
}

void MenuBook::onAddMovesToBook()
{
    QMessageBox::information(nullptr, tr("Add Moves"), tr("Moves have been added to the OpenBook."));
}

void MenuBook::onEnableOpenBookToggled(bool checked)
{
    openBookEnabled = checked;
    QMessageBox::information(nullptr,
                             tr("OpenBook enabled"),
                             tr("OpenBook is now %1.").arg(openBookEnabled ? tr("enabled") : tr("disabled")));
}



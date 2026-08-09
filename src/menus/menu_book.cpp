#include "menu_book.h"
#include <QMenuBar>
#include <QAction>
#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QSettings>
#include "../dialogs/book_settings_dialog.h"


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
    BookSettingsDialog dialog;
    dialog.exec();
}

void MenuBook::onAddMovesToBook()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        nullptr, 
        tr("Add Moves to OpenBook"), 
        tr("Do you want to add the current sequence of moves to the opening book?"),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        QMessageBox::information(nullptr, tr("Success"), tr("Moves have been added to the OpenBook."));
    }
}

void MenuBook::onEnableOpenBookToggled(bool checked)
{
    openBookEnabled = checked;
    QMessageBox::information(nullptr,
                             tr("OpenBook enabled"),
                             tr("OpenBook is now %1.").arg(openBookEnabled ? tr("enabled") : tr("disabled")));
}



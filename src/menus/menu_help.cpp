#include "menu_help.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QIcon>


void helpMenus(QMainWindow* window)
{
    QMenu *menuHelp = new QMenu(QObject::tr("Help"), window);

    QAction *doc = menuHelp->addAction(QIcon(":/icons/help_icon.png"), QObject::tr("Document"));
    doc->setShortcut(QKeySequence("F1"));

    QAction *about = menuHelp->addAction(QIcon(":/icons/about_icon.png"), QObject::tr("About..."));

    window->menuBar()->addMenu(menuHelp);

    QObject::connect(doc,   &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "Help"); });
    QObject::connect(about, &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "About"); });
}


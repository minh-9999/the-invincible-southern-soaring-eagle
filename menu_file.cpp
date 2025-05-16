
#include "menu_file.h"
#include "menu_utils.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QToolBar>
#include <QCoreApplication>


void fileMenus(QMainWindow* window)
{
    QMenu *menuFile = new QMenu(QCoreApplication::translate("MenuFile", "File"), window);

    QAction *actNew = menuFile->addAction(QIcon(":/icons/new_icon.png"), QCoreApplication::translate("MenuFile", "New"));
    actNew->setShortcut(QKeySequence("Ctrl+N"));

    QAction *actOpen = menuFile->addAction(QIcon(":/icons/open_icon.png"), QCoreApplication::translate("MenuFile", "Open..."));
    actOpen->setShortcut(QKeySequence("Ctrl+O"));

    QAction *actSave = menuFile->addAction(QIcon(":/icons/save_icon.png"), QCoreApplication::translate("MenuFile", "Save"));
    actSave->setShortcut(QKeySequence("Ctrl+S"));

    QAction *actSaveAs = menuFile->addAction(QIcon(":/icons/save_as_icon.png"), QCoreApplication::translate("MenuFile", "Save As..."));
    actSaveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));

    // menuFile->addSeparator();
    MenuUtils::addFullWidthSeparator(menuFile, 2);

    QAction *actExit = menuFile->addAction(QIcon(":/icons/exit_icon.png"), QCoreApplication::translate("MenuFile", "Exit"));
    actExit->setShortcut(QKeySequence("Ctrl+Q"));

    QAction *actGameProps = menuFile->addAction(QIcon(":/icons/properties_icon.png"), QCoreApplication::translate("MenuFile", "Game Properties"));
    actGameProps->setShortcut(QKeySequence("Ctrl+P"));

    window->menuBar()->addMenu(menuFile);


    QObject::connect(actNew,    &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "newGame"); });
    QObject::connect(actOpen,   &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "openGame"); });
    QObject::connect(actSave,   &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "saveGame"); });
    QObject::connect(actSaveAs, &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "saveGameAs"); });
    QObject::connect(actExit,   &QAction::triggered, window, [=]() { window->close(); });

    QObject::connect(actGameProps, &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "showGameProperties");
    });
}

void addFileActionsToToolBar(QToolBar *toolBar, QWidget *parent)
{
    QAction *actionNew = new QAction(QIcon(":/icons/new_icon.png"), "New", toolBar);
    QAction *actionOpen = new QAction(QIcon(":/icons/open_icon.png"), "Open...", toolBar);
    QAction *actionSave = new QAction(QIcon(":/icons/save_icon.png"), "Save", toolBar);
    QAction *actionSaveAs = new QAction(QIcon(":/icons/save_as_icon.png"), "Save As...", toolBar);

    toolBar->addAction(actionNew);
    toolBar->addAction(actionOpen);
    toolBar->addAction(actionSave);
    toolBar->addAction(actionSaveAs);

    QObject::connect(actionNew, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "newGame"); });
    QObject::connect(actionOpen, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "openGame"); });
    QObject::connect(actionSave, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "saveGame"); });
    QObject::connect(actionSaveAs, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "saveGameAs"); });
}

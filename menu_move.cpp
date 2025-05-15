#include "menu_move.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QMetaObject>
#include <QToolBar>


void moveMenus(QMainWindow* window)
{
    QMenu* menuMove = new QMenu("Move", window);

    QAction* actFirst  = menuMove->addAction(QIcon(":/icons/first_icon.png"), "Go to First");
    actFirst->setShortcut(QKeySequence("Ctrl+Up"));

    QAction* actLast   = menuMove->addAction(QIcon(":/icons/last_icon.png"), "Go to Last");
    actLast->setShortcut(QKeySequence("Ctrl+Down"));

    menuMove->addSeparator();

    QAction* actPrev   = menuMove->addAction(QIcon(":/icons/prev_icon.png"), "Step Back");
    actPrev->setShortcut(QKeySequence("Ctrl+Left"));

    QAction* actNext   = menuMove->addAction(QIcon(":/icons/next_icon.png"), "Step Forward");
    actNext->setShortcut(QKeySequence("Ctrl+Right"));

    menuMove->addSeparator();

    QAction* actTogglePlay = menuMove->addAction(QIcon(":/icons/play_icon.png"), "Play / Stop");
    actTogglePlay->setShortcut(QKeySequence("Ctrl+P"));

    QAction* actUndo = menuMove->addAction(QIcon(":/icons/undo_icon.png"), "Undo Move");
    actUndo->setShortcut(QKeySequence("Ctrl+U"));

    window->menuBar()->addMenu(menuMove);

    // Call slot by name if slot is in MainWindow
    QObject::connect(actFirst,  &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "goToFirst");
    });

    QObject::connect(actLast, &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "goToLast");
    });

    QObject::connect(actPrev, &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "stepBack");
    });

    QObject::connect(actNext, &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "stepForward");
    });

    QObject::connect(actTogglePlay, &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "togglePlayback");
    });

    QObject::connect(actUndo, &QAction::triggered, window, [=]() {
        QMetaObject::invokeMethod(window, "undoMove");
    });
}

void addMoveActionsToToolBar(QToolBar* toolbar, QWidget* parent)
{
    QAction* actFirst = new QAction(QIcon(":/icons/first_icon.png"), "Go to First", parent);
    toolbar->addAction(actFirst);
    QAction* actLast = new QAction(QIcon(":/icons/last_icon.png"), "Go to Last", parent);
    toolbar->addAction(actLast);

    toolbar->addSeparator();

    QAction* actPrev = new QAction(QIcon(":/icons/prev_icon.png"), "Step Back", parent);
    toolbar->addAction(actPrev);
    QAction* actNext = new QAction(QIcon(":/icons/next_icon.png"), "Step Forward", parent);
    toolbar->addAction(actNext);

    toolbar->addSeparator();

    QAction* actTogglePlay = new QAction(QIcon(":/icons/play_icon.png"), "Play / Stop", parent);
    toolbar->addAction(actTogglePlay);

    QAction* actUndo = new QAction(QIcon(":/icons/undo_icon.png"), "Undo Move", parent);
    toolbar->addAction(actUndo);

    QObject::connect(actFirst, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "goToFirst"); });
    QObject::connect(actLast, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "goToLast"); });
    QObject::connect(actPrev, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "stepBack"); });
    QObject::connect(actNext, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "stepForward"); });
    QObject::connect(actTogglePlay, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "togglePlayback"); });
    QObject::connect(actUndo, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "undoMove"); });
}

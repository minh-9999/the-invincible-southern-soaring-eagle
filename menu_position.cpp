
#include "menu_position.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QToolBar>

void positionMenus(QMainWindow* window)
{
    // menu Position
    QMenu *menuPosition = new QMenu("Position", window);

    QAction *actEditPosition  = menuPosition->addAction(QIcon(":/icons/edit_icon.png"), "Edit Position");
    actEditPosition->setShortcut(QKeySequence("Ctrl+E"));
    menuPosition->addSeparator();
    QAction *actReverseBoard  = menuPosition->addAction(QIcon(":/icons/reverse_icon.png"), "Reverse Position");
    actReverseBoard->setShortcut(QKeySequence("Ctrl+R"));

    menuPosition->addSeparator();
    QAction *actCopyPosition  = menuPosition->addAction(QIcon(":/icons/copy_icon.png"), "Copy Position");
    actCopyPosition->setShortcut(QKeySequence("Ctrl+C"));

    QAction *actCopyMoves     = menuPosition->addAction(QIcon(":/icons/copy_moves_icon.png"), "Copy MoveList");
    actCopyMoves->setShortcut(QKeySequence("Ctrl+Shift+C"));

    QAction *actPastePosition = menuPosition->addAction(QIcon(":/icons/paste_icon.png"), "Paste Position/MoveList");
    actPastePosition->setShortcut(QKeySequence("Ctrl+V"));

    window->menuBar()->addMenu(menuPosition);

    // connect signal
    QObject::connect(actEditPosition,  &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "editPosition"); });
    QObject::connect(actReverseBoard,  &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "reverseBoard"); });
    QObject::connect(actCopyPosition,  &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "copyPosition"); });
    QObject::connect(actCopyMoves,     &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "copyMoveList"); });
    QObject::connect(actPastePosition, &QAction::triggered, window, [=]() { QMetaObject::invokeMethod(window, "pastePosition"); });
}

void addPositionActionsToToolBar(QToolBar* toolbar, QWidget* parent)
{
    QAction *actEditPosition  = new QAction(QIcon(":/icons/edit_icon.png"), "Edit Position", parent);
    toolbar->addAction(actEditPosition);
    toolbar->addSeparator();

    QAction *actReverseBoard  = new QAction(QIcon(":/icons/reverse_icon.png"), "Reverse Position", parent);
    toolbar->addAction(actReverseBoard);
    toolbar->addSeparator();

    QAction *actCopyPosition  = new QAction(QIcon(":/icons/copy_icon.png"), "Copy Position", parent);
    toolbar->addAction(actCopyPosition);
    QAction *actCopyMoves     = new QAction(QIcon(":/icons/copy_moves_icon.png"), "Copy MoveList", parent);
    toolbar->addAction(actCopyMoves);
    QAction *actPastePosition = new QAction(QIcon(":/icons/paste_icon.png"), "Paste Position/MoveList", parent);
    toolbar->addAction(actPastePosition);


    QObject::connect(actEditPosition,  &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "editPosition"); });
    QObject::connect(actReverseBoard,  &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "reverseBoard"); });
    QObject::connect(actCopyPosition,  &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "copyPosition"); });
    QObject::connect(actCopyMoves,     &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "copyMoveList"); });
    QObject::connect(actPastePosition, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "pastePosition"); });
}

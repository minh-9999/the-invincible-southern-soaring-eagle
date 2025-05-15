#include "menu_view.h"
#include "menu_utils.h"


MenuView::MenuView(QObject* parent): QObject(parent)
{
}

void MenuView::viewMenus(QMainWindow* window)
{
    QMenu* viewMenu = new QMenu("View", window);

    QAction* toolbarAction = viewMenu->addAction("ToolBar");
    // toolbarAction = new QAction("ToolBar", this);
    toolbarAction->setCheckable(true);
    toolbarAction->setChecked(true);

    QAction* statusBarAction = viewMenu->addAction("StatusBar");
    // statusBarAction = new QAction("StatusBar", this);
    statusBarAction->setCheckable(true);
    statusBarAction->setChecked(true);

    viewMenu->addSeparator();
    // MenuUtils::addFullWidthSeparator(viewMenu, 2);

    QAction* moveListAction = viewMenu->addAction("MoveList Window");
    // moveListAction = new QAction("MoveList Window", this);
    moveListAction->setCheckable(true);
    moveListAction->setChecked(true);

    QAction* engineWindowAction = viewMenu->addAction("Engine Window");
    // engineWindowAction = new QAction("Engine Window", this);
    engineWindowAction->setCheckable(true);
    engineWindowAction->setChecked(true);

    QAction* openBookWindowAction = viewMenu->addAction("OpenBook Window");
    // openBookWindowAction = new QAction("OpenBook Window", this);
    openBookWindowAction->setCheckable(true);
    openBookWindowAction->setChecked(true);

    QAction* secondaryBookWindowAction = viewMenu->addAction("SecondaryBook Window");
    // secondaryBookWindowAction = new QAction("SecondaryBook Window", this);
    secondaryBookWindowAction->setCheckable(true);
    secondaryBookWindowAction->setChecked(true);

    QAction* cloudBookWindowAction = viewMenu->addAction("CloudBook Window");
    // cloudBookWindowAction = new QAction("CloudBook Window", this);
    cloudBookWindowAction->setCheckable(true);
    cloudBookWindowAction->setChecked(true);

    viewMenu->addSeparator();
    // MenuUtils::addFullWidthSeparator(viewMenu, 6);

    QMenu* schemeMenu = viewMenu->addMenu("Window Scheme");

    QAction* saveSchemeAction = schemeMenu->addAction("Save Current Scheme");
    // saveSchemeAction = new QAction("Save Current Scheme", this);
    QAction* deleteSchemeAction = schemeMenu->addAction("Delete A Scheme");
    // deleteSchemeAction = new QAction("Delete A Scheme", this);
    QAction* restoreDefaultSchemeAction = schemeMenu->addAction("Restore Default Scheme");
    // restoreDefaultSchemeAction = new QAction("Restore Default Scheme", this);

    window->menuBar()->addMenu(viewMenu);

    // viewMenu->addAction(toolbarAction);
    // viewMenu->addAction(statusBarAction);
    // viewMenu->addSeparator();
    // viewMenu->addAction(moveListAction);
    // viewMenu->addAction(engineWindowAction);
    // viewMenu->addAction(openBookWindowAction);
    // viewMenu->addAction(secondaryBookWindowAction);
    // viewMenu->addAction(cloudBookWindowAction);
    // viewMenu->addSeparator();

    // schemeMenu->addAction(saveSchemeAction);
    // schemeMenu->addAction(deleteSchemeAction);
    // schemeMenu->addAction(restoreDefaultSchemeAction);

    connect(toolbarAction, &QAction::toggled, this, &MenuView::toggleToolBar);
    connect(statusBarAction, &QAction::toggled, this, &MenuView::toggleStatusBar);
    connect(moveListAction, &QAction::toggled, this, &MenuView::toggleMoveListWindow);
    connect(engineWindowAction, &QAction::toggled, this, &MenuView::toggleEngineWindow);
    connect(openBookWindowAction, &QAction::toggled, this, &MenuView::toggleOpenBookWindow);
    connect(secondaryBookWindowAction, &QAction::toggled, this, &MenuView::toggleSecondaryBookWindow);
    connect(cloudBookWindowAction, &QAction::toggled, this, &MenuView::toggleCloudBookWindow);
    connect(saveSchemeAction, &QAction::triggered, this, &MenuView::saveCurrentScheme);
    connect(deleteSchemeAction, &QAction::triggered, this, &MenuView::deleteScheme);
    connect(restoreDefaultSchemeAction, &QAction::triggered, this, &MenuView::restoreDefaultScheme);

}

// QMenu* MenuView::getMenu() const
// {
//     // return viewMenus;
// }

void MenuView::toggleToolBar(bool checked)
{
    // Handles toolbar toggle
    if (checked) {
        // Shows toolbar
    }
    else {
        // Hides toolbar
    }
}

void MenuView::toggleStatusBar(bool checked)
{
    // Handles status bar toggle
    if (checked) {
        // Shows status bar
    }
    else {
        // Hides status bar
    }
}

void MenuView::toggleMoveListWindow(bool checked)
{
    // Handles MoveList window toggle
    if (checked) {
        // Shows MoveList window
    }
    else {
        // Hides MoveList window
    }
}

void MenuView::toggleEngineWindow(bool checked)
{
    // Handles Engine window toggle
    if (checked) {
        // Shows window Engine
    }
    else {
        // Hide Engine window
    }
}

void MenuView::toggleOpenBookWindow(bool checked)
{
    // Handle OpenBook window on/off
    if (checked) {
        // Show OpenBook window
    }
    else {
        // Hide OpenBook window
    }
}

void MenuView::toggleSecondaryBookWindow(bool checked)
{
    // Handle SecondaryBook window on/off
    if (checked) {
        // Show SecondaryBook window
    }
    else {
        // Hide SecondaryBook window
    }
}

void MenuView::toggleCloudBookWindow(bool checked)
{
    // Handle CloudBook window on/off
    if (checked) {
        // Show CloudBook window
    }
    else {
        // Hide CloudBook window
    }
}

void MenuView::saveCurrentScheme()
{
    // Handles saving the current scheme
}

void MenuView::deleteScheme()
{
    // Handles deleting the scheme
}

void MenuView::restoreDefaultScheme()
{
    // Handles restoring the default scheme
}

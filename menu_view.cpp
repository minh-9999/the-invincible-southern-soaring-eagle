#include "menu_view.h"
#include "menu_utils.h"

#include <QStatusBar>


MenuView::MenuView(QMainWindow* mWindow, QWidget* parent)
    : QObject(parent), mMainWindow(mWindow)
{

}

void MenuView::viewMenus(QMainWindow* window)
{
    QMenu* viewMenu = new QMenu(tr("View"), window);

    QAction* toolbarAction = viewMenu->addAction(tr("ToolBar"));
    toolbarAction->setCheckable(true);
    toolbarAction->setChecked(true);

    QAction* statusBarAction = viewMenu->addAction(tr("StatusBar"));
    statusBarAction->setCheckable(true);
    statusBarAction->setChecked(true);

    // viewMenu->addSeparator();
    MenuUtils::addFullWidthSeparator(viewMenu, 2);

    QAction* moveListAction = viewMenu->addAction(tr("MoveList Window"));
    moveListAction->setCheckable(true);
    moveListAction->setChecked(true);

    QAction* engineWindowAction = viewMenu->addAction(tr("Engine Window"));
    engineWindowAction->setCheckable(true);
    engineWindowAction->setChecked(true);

    QAction* openBookWindowAction = viewMenu->addAction(tr("OpenBook Window"));
    openBookWindowAction->setCheckable(true);
    openBookWindowAction->setChecked(true);

    QAction* secondaryBookWindowAction = viewMenu->addAction(tr("SecondaryBook Window"));
    secondaryBookWindowAction->setCheckable(true);
    secondaryBookWindowAction->setChecked(true);

    QAction* cloudBookWindowAction = viewMenu->addAction(tr("CloudBook Window"));
    cloudBookWindowAction->setCheckable(true);
    cloudBookWindowAction->setChecked(true);

    // viewMenu->addSeparator();
    MenuUtils::addFullWidthSeparator(viewMenu, 2);

    QMenu* schemeMenu = viewMenu->addMenu(tr("Window Scheme"));

    QAction* saveSchemeAction = schemeMenu->addAction(tr("Save Current Scheme"));
    QAction* deleteSchemeAction = schemeMenu->addAction(tr("Delete A Scheme"));
    QAction* restoreDefaultSchemeAction = schemeMenu->addAction(tr("Restore Default Scheme"));

    window->menuBar()->addMenu(viewMenu);

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


void MenuView::bindToolBars(CustomToolBar* tbar1, CustomToolBar_2* tbar2)
{
    mtoolbar_1 = tbar1;
    mtoolbar_2 = tbar2;
}

// QMenu* MenuView::getMenu() const
// {
//     // return viewMenus;
// }

void MenuView::toggleToolBar(bool checked)
{
    if (mtoolbar_1 && mtoolbar_2)
    {
        mtoolbar_1->setVisible(checked);
        mtoolbar_2->setVisible(checked);
    }
}


void MenuView::toggleStatusBar(bool checked)
{
    if (!mMainWindow) return;

    checked ? mMainWindow->statusBar()->show() :
            mMainWindow->statusBar()->hide();
}

void MenuView::toggleMoveListWindow(bool checked)
{
    // Handles MoveList window toggle
    if (checked)
    {
        // Shows MoveList window
    }
    else {
        // Hides MoveList window
    }
}

void MenuView::toggleEngineWindow(bool checked)
{
    // Handles Engine window toggle
    if (checked)
    {
        // Shows window Engine
    }
    else
    {
        // Hide Engine window
    }
}

void MenuView::toggleOpenBookWindow(bool checked)
{
    // Handle OpenBook window on/off
    if (checked)
    {
        // Show OpenBook window
    }
    else
    {
        // Hide OpenBook window
    }
}

void MenuView::toggleSecondaryBookWindow(bool checked)
{
    // Handle SecondaryBook window on/off
    if (checked)
    {
        // Show SecondaryBook window
    }
    else
    {
        // Hide SecondaryBook window
    }
}

void MenuView::toggleCloudBookWindow(bool checked)
{
    // Handle CloudBook window on/off
    if (checked)
    {
        // Show CloudBook window
    }
    else
    {
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

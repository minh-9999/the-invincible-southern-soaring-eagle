#include "menu_view.h"
#include "menu_utils.h"

#include <QStatusBar>
#include <QMessageBox>
#include "../chess/CustomToolBar.h"


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
    if (checked) {
        QMessageBox::information(nullptr, tr("MoveList Window"), tr("MoveList panel is not yet implemented."));
    }
}

void MenuView::toggleEngineWindow(bool checked)
{
    if (checked) {
        QMessageBox::information(nullptr, tr("Engine Window"), tr("Engine panel is not yet implemented."));
    }
}

void MenuView::toggleOpenBookWindow(bool checked)
{
    if (checked) {
        QMessageBox::information(nullptr, tr("OpenBook Window"), tr("OpenBook panel is not yet implemented."));
    }
}

void MenuView::toggleSecondaryBookWindow(bool checked)
{
    if (checked) {
        QMessageBox::information(nullptr, tr("SecondaryBook Window"), tr("SecondaryBook panel is not yet implemented."));
    }
}

void MenuView::toggleCloudBookWindow(bool checked)
{
    if (checked) {
        QMessageBox::information(nullptr, tr("CloudBook Window"), tr("CloudBook panel is not yet implemented."));
    }
}

void MenuView::saveCurrentScheme()
{
    QMessageBox::information(nullptr, tr("Window Scheme"), tr("Save Scheme is not yet implemented."));
}

void MenuView::deleteScheme()
{
    QMessageBox::information(nullptr, tr("Window Scheme"), tr("Delete Scheme is not yet implemented."));
}

void MenuView::restoreDefaultScheme()
{
    QMessageBox::information(nullptr, tr("Window Scheme"), tr("Restore Default Scheme is not yet implemented."));
}

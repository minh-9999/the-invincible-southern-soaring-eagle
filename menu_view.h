#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#include <QMenuBar>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QObject>

#include "CustomToolBar_2.h"

class MenuView: public QObject
{
    Q_OBJECT

public:
    explicit MenuView(QMainWindow* mWindow, QWidget *parent = nullptr);

    void viewMenus(QMainWindow* window);

    void bindToolBars(CustomToolBar* tbar1, CustomToolBar_2* tbar2);

    // QMenu* getMenu() const;

    // QMenu* viewMenu;
    // QMenu* schemeMenu;
    // QAction* toolbarAction;
    // QAction* statusBarAction;
    // QAction* moveListAction;
    // QAction* engineWindowAction;
    // QAction* openBookWindowAction;
    // QAction* secondaryBookWindowAction;
    // QAction* cloudBookWindowAction;

    // QAction* saveSchemeAction;
    // QAction* deleteSchemeAction;
    // QAction* restoreDefaultSchemeAction;

private slots:
    void toggleToolBar(bool checked);
    void toggleStatusBar(bool checked);
    void toggleMoveListWindow(bool checked);
    void toggleEngineWindow(bool checked);
    void toggleOpenBookWindow(bool checked);
    void toggleSecondaryBookWindow(bool checked);
    void toggleCloudBookWindow(bool checked);

    void saveCurrentScheme();
    void deleteScheme();
    void restoreDefaultScheme();

private:

    CustomToolBar *mtoolbar_1;
    CustomToolBar_2 *mtoolbar_2;

    QMainWindow* mMainWindow;
};

#endif // MENU_VIEW_H

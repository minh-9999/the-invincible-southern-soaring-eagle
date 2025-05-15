#ifndef MENU_VIEW_H
#define MENU_VIEW_H

#include <QMenuBar>
#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QObject>

class MenuView: public QObject
{
    Q_OBJECT

public:
    explicit MenuView(QObject *parent = nullptr);
    void viewMenus(QMainWindow* window);

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
};

#endif // MENU_VIEW_H

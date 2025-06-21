#ifndef MENU_ENGINE_H
#define MENU_ENGINE_H

#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QMainWindow>
#include <QObject>


class ChessBoardWidget;  // Forward declaration

class MenuEngine : public QObject
{
    Q_OBJECT

public:
    explicit MenuEngine(ChessBoardWidget *bWidget);

    // explicit MenuEngine(QObject *parent = nullptr);

    void engineMenus(QMainWindow* window);

    void addEngineActionsToToolBar(QToolBar *toolbar, QWidget* parent);


private slots:
    void onEngineSetting();
    void onEngineBlack();
    void onEngineRed();
    void onAnalyzeMode();

private:
    ChessBoardWidget *boardWidget; // Reference to ChessBoardWidget
};

#endif // MENU_ENGINE_H

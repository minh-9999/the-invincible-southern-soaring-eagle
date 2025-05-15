#include "menu_engine.h"
#include "ChessBoardWidget.h"  // Include to access ChessBoardWidget

#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QMenuBar>
#include <QToolBar>


MenuEngine::MenuEngine(ChessBoardWidget *bWidget)
    : QObject(), boardWidget(bWidget)
{
}

// MenuEngine::MenuEngine(QObject* parent): QObject(parent)
// {
// }

void MenuEngine::engineMenus(QMainWindow* window)
{
    // Engine menu
    QMenu *engineMenu = new QMenu("Engine", window);

    QAction *engineSettingAct = engineMenu->addAction(QIcon(":/icons/setting_icon.png"), "Engine Setting");
    engineSettingAct->setShortcut(QKeySequence("Ctrl+K"));

    QAction *engineBlackAct = engineMenu->addAction(QIcon(":/icons/black_icon.png"), "Engine Black");
    engineBlackAct->setShortcut(QKeySequence("Ctrl+B"));

    QAction *engineRedAct = engineMenu->addAction(QIcon(":/icons/red_icon.png"), "Engine Red");
    engineRedAct->setShortcut(QKeySequence("Ctrl+R"));

    QAction *analyzeModeAct = engineMenu->addAction(QIcon(":/icons/analyze_icon.png"), "Analyze Mode");
    analyzeModeAct->setShortcut(QKeySequence("Ctrl+A"));

    // ✅ Reuse actions from boardWidget
    if (boardWidget)
    {
        if (boardWidget->actChangeMove)
            engineMenu->addAction(boardWidget->actChangeMove);

        else
            qWarning() << "[MenuEngine] actChangeMove is nullptr!";

        if (boardWidget->actMoveNow)
            engineMenu->addAction(boardWidget->actMoveNow);

        else
            qWarning() << "[MenuEngine] actMoveNow is nullptr!";
    }
    else
    {
        qCritical() << "[MenuEngine] boardWidget is nullptr!";
    }

    window->menuBar()->addMenu(engineMenu);

    // Connect actions to slots
    connect(engineSettingAct, &QAction::triggered, this, &MenuEngine::onEngineSetting);
    connect(engineBlackAct, &QAction::triggered, this, &MenuEngine::onEngineBlack);
    connect(engineRedAct, &QAction::triggered, this, &MenuEngine::onEngineRed);
    connect(analyzeModeAct, &QAction::triggered, this, &MenuEngine::onAnalyzeMode);
    // connect(changeMoveAct, &QAction::triggered, this, &MenuEngine::onChangeMove);
    // connect(moveNowAct, &QAction::triggered, this, &MenuEngine::onMoveNow);


    // Show the menu
    // menu.exec(boardWidget->mapToGlobal(pos)); // Show the context menu at mouse position
}

void MenuEngine::addEngineActionsToToolBar(QToolBar *toolbar, QWidget* parent)
{
    QAction *engineSettingAct = new QAction(QIcon(":/icons/setting_icon.png"), "Engine Setting", parent);
    toolbar->addAction(engineSettingAct);

    toolbar->addSeparator();
    QAction *engineBlackAct = new QAction(QIcon(":/icons/black_icon.png"), "Engine Black", parent);
    toolbar->addAction(engineBlackAct);

    QAction *engineRedAct = new QAction(QIcon(":/icons/red_icon.png"), "Engine Red", parent);
    toolbar->addAction(engineRedAct);

    toolbar->addSeparator();

    QAction *analyzeModeAct = new QAction(QIcon(":/icons/analyze_icon.png"), "Analyze Mode", parent);
    toolbar->addAction(analyzeModeAct);

    if (boardWidget)
    {
        qDebug() << "[MenuEngine] boardWidget ptr =" << boardWidget;

        if (boardWidget->actChangeMove)
            toolbar->addAction(boardWidget->actChangeMove);

        else
            qWarning() << "[MenuEngine] actChangeMove is nullptr!";

        if (boardWidget->actMoveNow)
            toolbar->addAction(boardWidget->actMoveNow);

        else
            qWarning() << "[MenuEngine] actMoveNow is nullptr!";
    }
    else
    {
        qCritical() << "[MenuEngine] boardWidget is nullptr!";
    }

    QObject::connect(engineSettingAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onEngineSetting"); });
    QObject::connect(engineBlackAct, &QAction::triggered, parent, [=]()  { QMetaObject::invokeMethod(parent, "onEngineBlack"); });
    QObject::connect(engineRedAct, &QAction::triggered, parent, [=]()  { QMetaObject::invokeMethod(parent, "onEngineRed"); });
    QObject::connect(analyzeModeAct, &QAction::triggered, parent, [=]()  { QMetaObject::invokeMethod(parent, "onAnalyzeMode"); });
}

void MenuEngine::onEngineSetting()
{
    qDebug() << "Engine Settings triggered!";
    // Call method in ChessBoardWidget to handle engine settings
    boardWidget->onEngineSetting();
}

void MenuEngine::onEngineBlack()
{
    qDebug() << "Engine Black triggered!";
    // Set engine for Black side
    boardWidget->onEngineBlack();
}

void MenuEngine::onEngineRed()
{
    qDebug() << "Engine Red triggered!";
    // Set engine for Red side
    boardWidget->onEngineRed();
}

void MenuEngine::onAnalyzeMode()
{
    qDebug() << "Analyze Mode triggered!";
    // Trigger the Analyze Mode
    boardWidget->requestAnalyzeMode();
}

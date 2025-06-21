#include "menu_engine.h"
#include "ChessBoardWidget.h"  // Include to access ChessBoardWidget
#include "engine_settings_dialog.h"

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
    QMenu *engineMenu = new QMenu(tr("Engine"), window);

    QAction *engineSettingAct = engineMenu->addAction(QIcon(":/icons/setting_icon.png"), tr("Engine Setting"));
    engineSettingAct->setShortcut(QKeySequence("Ctrl+K"));

    QAction *engineBlackAct = engineMenu->addAction(QIcon(":/icons/black_icon.png"), tr("Engine Black"));
    engineBlackAct->setShortcut(QKeySequence("Ctrl+B"));

    QAction *engineRedAct = engineMenu->addAction(QIcon(":/icons/red_icon.png"), tr("Engine Red"));
    engineRedAct->setShortcut(QKeySequence("Ctrl+R"));

    QAction *analyzeModeAct = engineMenu->addAction(QIcon(":/icons/analyze_icon.png"), tr("Analyze Mode"));
    analyzeModeAct->setShortcut(QKeySequence("Ctrl+A"));

    // ✅ Reuse actions from boardWidget
    engineMenu->addAction(boardWidget->actChangeMove);
    engineMenu->addAction(boardWidget->actMoveNow);

    window->menuBar()->addMenu(engineMenu);

    // Connect actions to slots
    connect(engineSettingAct, &QAction::triggered, this, &MenuEngine::onEngineSetting);
    connect(engineBlackAct, &QAction::triggered, this, &MenuEngine::onEngineBlack);
    connect(engineRedAct, &QAction::triggered, this, &MenuEngine::onEngineRed);
    connect(analyzeModeAct, &QAction::triggered, this, &MenuEngine::onAnalyzeMode);
    // connect(changeMoveAct, &QAction::triggered, this, &MenuEngine::onChangeMove);
    // connect(moveNowAct, &QAction::triggered, this, &MenuEngine::onMoveNow);

}

void MenuEngine::addEngineActionsToToolBar(QToolBar *toolbar, QWidget* parent)
{
    QAction *engineSettingAct = new QAction(QIcon(":/icons/setting_icon.png"), tr("Engine Setting"), parent);
    toolbar->addAction(engineSettingAct);

    toolbar->addSeparator();

    QAction *engineBlackAct = new QAction(QIcon(":/icons/black_icon.png"), tr("Engine Black"), parent);
    toolbar->addAction(engineBlackAct);

    QAction *engineRedAct = new QAction(QIcon(":/icons/red_icon.png"), tr("Engine Red"), parent);
    toolbar->addAction(engineRedAct);

    toolbar->addSeparator();

    QAction *analyzeModeAct = new QAction(QIcon(":/icons/analyze_icon.png"), tr("Analyze Mode"), parent);
    toolbar->addAction(analyzeModeAct);

    toolbar->addAction(boardWidget->actChangeMove);
    toolbar->addAction(boardWidget->actMoveNow);

    QObject::connect(engineSettingAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onEngineSetting"); });
    QObject::connect(engineBlackAct, &QAction::triggered, parent, [=]()  { QMetaObject::invokeMethod(parent, "onEngineBlack"); });
    QObject::connect(engineRedAct, &QAction::triggered, parent, [=]()  { QMetaObject::invokeMethod(parent, "onEngineRed"); });
    QObject::connect(analyzeModeAct, &QAction::triggered, parent, [=]()  { QMetaObject::invokeMethod(parent, "onAnalyzeMode"); });
}

void MenuEngine::onEngineSetting()
{
    qDebug() << "Engine Settings triggered!";

    // boardWidget->onEngineSetting();

    EngineSettingsDialog dialog(boardWidget);
    dialog.exec(); // Show as modal dialog
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

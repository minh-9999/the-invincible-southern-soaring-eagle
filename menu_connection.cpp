#include "menu_connection.h"
#include "menu_utils.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>

MenuConnection::MenuConnection(QObject* parent)
    : QObject(parent) {}


void MenuConnection::connectionMenus(QMainWindow* window)
{
    QMenu* connectionMenu = new QMenu(tr("Connection"), window);

    QAction* settingsAct = connectionMenu->addAction(QIcon(":/icons/connection_settings_icon.png"), tr("Connection Settings..."));
    settingsAct->setShortcut(QKeySequence("Ctrl+T"));

    QAction* newConnAct = connectionMenu->addAction(QIcon(":/icons/connection_add_icon.png"), tr("New Connection..."));
    newConnAct->setShortcut(QKeySequence("Ctrl+Shift+N"));

    QAction* editConnAct = connectionMenu->addAction(QIcon(":/icons/edit_connection_icon.png"), tr("Edit Connection..."));
    editConnAct->setShortcut(QKeySequence("Ctrl+X"));

    QAction* connectToAct = connectionMenu->addAction(QIcon(":/icons/connect_to_icon.png"), tr("Connect To"));
    connectToAct->setShortcut(QKeySequence("Ctrl+Q"));

    QAction* debugInfoAct = connectionMenu->addAction(QIcon(":/icons/debug_icon.png"), tr("Show Debug Info"));
    debugInfoAct->setShortcut(QKeySequence("Ctrl+D"));

    // connectionMenu->addSeparator();
    MenuUtils::addFullWidthSeparator(connectionMenu, 2);

    QAction* manualModeAct = connectionMenu->addAction(QIcon(":/icons/manual_icon.png"), tr("Manual Mode"));
    manualModeAct->setShortcut(QKeySequence("Ctrl+Shift+M"));

    QAction* transmitAct = connectionMenu->addAction(QIcon(":/icons/transmit_icon.png"), tr("Transmit To Game"));
    transmitAct->setShortcut(Qt::Key_F5);

    QAction* autoClickAct = connectionMenu->addAction(QIcon(":/icons/auto_click_icon.png"), tr("Auto Click"));
    autoClickAct->setShortcut(QKeySequence("Ctrl+Shift+C"));

    window->menuBar()->addMenu(connectionMenu);

    connect(settingsAct, &QAction::triggered, this, &MenuConnection::onConnectionSettings);
    connect(newConnAct, &QAction::triggered, this, &MenuConnection::onNewConnection);
    connect(editConnAct, &QAction::triggered, this, &MenuConnection::onEditConnection);
    connect(connectToAct, &QAction::triggered, this, &MenuConnection::onConnectTo);
    connect(debugInfoAct, &QAction::triggered, this, &MenuConnection::onShowDebugInfo);
    connect(manualModeAct, &QAction::triggered, this, &MenuConnection::onManualMode);
    connect(transmitAct, &QAction::triggered, this, &MenuConnection::onTransmitToGame);
    connect(autoClickAct, &QAction::triggered, this, &MenuConnection::onAutoClick);
}

void MenuConnection::addConnectActionsToToolBar(QToolBar *toolBar, QWidget *parent)
{
    QAction* settingsAct = new QAction(QIcon(":/icons/connection_settings_icon.png"), tr("Connection Settings..."), parent);
    toolBar->addAction(settingsAct);

    QAction* newConnAct = new QAction(QIcon(":/icons/connection_add_icon.png"), tr("New Connection..."), parent);
    toolBar->addAction(newConnAct);

    QAction* editConnAct = new QAction(QIcon(":/icons/edit_connection_icon.png"), tr("Edit Connection..."), parent);
    toolBar->addAction(editConnAct);

    QAction* connectToAct = new QAction(QIcon(":/icons/connect_to_icon.png"), tr("Connect To"), parent);
    toolBar->addAction(connectToAct);

    QAction* debugInfoAct = new QAction(QIcon(":/icons/debug_icon.png"), tr("Show Debug Info"), parent);
    toolBar->addAction(debugInfoAct);

    toolBar->addSeparator();

    QAction* manualModeAct = new QAction(QIcon(":/icons/manual_icon.png"), tr("Manual Mode"), parent);
    toolBar->addAction(manualModeAct);

    QAction* transmitAct = new QAction(QIcon(":/icons/transmit_icon.png"), tr("Transmit To Game"), parent);
    toolBar->addAction(transmitAct);

    QAction* autoClickAct = new QAction(QIcon(":/icons/auto_click_icon.png"), tr("Auto Click"), parent);
    toolBar->addAction(autoClickAct);

    QObject::connect(settingsAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onConnectionSettings"); });
    QObject::connect(newConnAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onNewConnection"); });
    QObject::connect(editConnAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onEditConnection"); });
    QObject::connect(connectToAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onConnectTo"); });
    QObject::connect(debugInfoAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onShowDebugInfo"); });
    QObject::connect(manualModeAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onManualMode"); });
    QObject::connect(transmitAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onTransmitToGame"); });
    QObject::connect(autoClickAct, &QAction::triggered, parent, [=]() { QMetaObject::invokeMethod(parent, "onAutoClick"); });

}


void MenuConnection::onConnectionSettings()
{
    QMessageBox::information(nullptr, tr("Settings"), tr("Open connection settings."));
}

void MenuConnection::onNewConnection()
{
    QMessageBox::information(nullptr, tr("New"), tr("Create a new connection."));
}

void MenuConnection::onEditConnection()
{
    QMessageBox::information(nullptr, tr("Edit"), tr("Edit an existing connection."));
}

void MenuConnection::onConnectTo()
{
    QMessageBox::information(nullptr, tr("Connect"), tr("Connecting to game..."));
}

void MenuConnection::onShowDebugInfo()
{
    QMessageBox::information(nullptr, tr("Debug Info"), tr("Showing debug information."));
}

void MenuConnection::onManualMode()
{
    QMessageBox::information(nullptr, tr("Manual Mode"), tr("Switched to manual mode."));
}

void MenuConnection::onTransmitToGame()
{
    QMessageBox::information(nullptr, tr("Transmit"), tr("Transmitting data to game."));
}

void MenuConnection::onAutoClick()
{
    QMessageBox::information(nullptr, tr("Auto Click"), tr("Auto click activated."));
}


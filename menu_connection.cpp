#include "menu_connection.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>

MenuConnection::MenuConnection(QObject* parent)
    : QObject(parent) {}


void MenuConnection::connectionMenus(QMainWindow* window)
{
    QMenu* connectionMenu = new QMenu("Connection", window);

    QAction* settingsAct = connectionMenu->addAction(QIcon(":/icons/connection_settings_icon.png"), "Connection Settings...");
    settingsAct->setShortcut(QKeySequence("Ctrl+T"));

    QAction* newConnAct = connectionMenu->addAction(QIcon(":/icons/connection_add_icon.png"), "New Connection...");
    newConnAct->setShortcut(QKeySequence("Ctrl+Shift+N"));

    QAction* editConnAct = connectionMenu->addAction(QIcon(":/icons/edit_connection_icon.png"), "Edit Connection...");
    editConnAct->setShortcut(QKeySequence("Ctrl+X"));

    QAction* connectToAct = connectionMenu->addAction(QIcon(":/icons/connect_to_icon.png"), "Connect To");
    connectToAct->setShortcut(QKeySequence("Ctrl+Q"));

    QAction* debugInfoAct = connectionMenu->addAction(QIcon(":/icons/debug_icon.png"), "Show Debug Info");
    debugInfoAct->setShortcut(QKeySequence("Ctrl+D"));

    connectionMenu->addSeparator();

    QAction* manualModeAct = connectionMenu->addAction(QIcon(":/icons/manual_icon.png"), "Manual Mode");
    manualModeAct->setShortcut(QKeySequence("Ctrl+Shift+M"));

    QAction* transmitAct = connectionMenu->addAction(QIcon(":/icons/transmit_icon.png"), "Transmit To Game");
    transmitAct->setShortcut(Qt::Key_F5);

    QAction* autoClickAct = connectionMenu->addAction(QIcon(":/icons/auto_click_icon.png"), "Auto Click");
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
    QAction* settingsAct = new QAction(QIcon(":/icons/connection_settings_icon.png"), "Connection Settings...", parent);
    toolBar->addAction(settingsAct);

    QAction* newConnAct =  new QAction(QIcon(":/icons/connection_add_icon.png"), "New Connection...", parent);
    toolBar->addAction(newConnAct);

    QAction* editConnAct =  new QAction(QIcon(":/icons/edit_connection_icon.png"), "Edit Connection...", parent);
    toolBar->addAction(editConnAct);

    QAction* connectToAct =  new QAction(QIcon(":/icons/connect_to_icon.png"), "Connect To", parent);
    toolBar->addAction(connectToAct);

    QAction* debugInfoAct =  new QAction(QIcon(":/icons/debug_icon.png"), "Show Debug Info", parent);
    toolBar->addAction(debugInfoAct);

    toolBar->addSeparator();

    QAction* manualModeAct =  new QAction(QIcon(":/icons/manual_icon.png"), "Manual Mode", parent);
    toolBar->addAction(manualModeAct);

    QAction* transmitAct =  new QAction(QIcon(":/icons/transmit_icon.png"), "Transmit To Game", parent);
    toolBar->addAction(transmitAct);

    QAction* autoClickAct =  new QAction(QIcon(":/icons/auto_click_icon.png"), "Auto Click", parent);
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
    QMessageBox::information(nullptr, "Settings", "Open connection settings.");
}

void MenuConnection::onNewConnection()
{
    QMessageBox::information(nullptr, "New", "Create a new connection.");
}

void MenuConnection::onEditConnection()
{
    QMessageBox::information(nullptr, "Edit", "Edit an existing connection.");
}

void MenuConnection::onConnectTo()
{
    QMessageBox::information(nullptr, "Connect", "Connecting to game...");
}

void MenuConnection::onShowDebugInfo()
{
    QMessageBox::information(nullptr, "Debug Info", "Showing debug information.");
}

void MenuConnection::onManualMode()
{
    QMessageBox::information(nullptr, "Manual Mode", "Switched to manual mode.");
}

void MenuConnection::onTransmitToGame()
{
    QMessageBox::information(nullptr, "Transmit", "Transmitting data to game.");
}

void MenuConnection::onAutoClick()
{
    QMessageBox::information(nullptr, "Auto Click", "Auto click activated.");
}

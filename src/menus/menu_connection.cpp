#include "menu_connection.h"
#include "menu_utils.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QToolBar>

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>

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
    QDialog dialog;
    dialog.setWindowTitle(tr("Connection Settings"));
    dialog.setMinimumWidth(300);

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *ipEdit = new QLineEdit("127.0.0.1", &dialog);
    layout->addRow(tr("IP Address:"), ipEdit);

    QSpinBox *portSpin = new QSpinBox(&dialog);
    portSpin->setRange(1, 65535);
    portSpin->setValue(8080);
    layout->addRow(tr("Port:"), portSpin);

    QComboBox *protocolCombo = new QComboBox(&dialog);
    protocolCombo->addItems({"TCP", "UDP"});
    layout->addRow(tr("Protocol:"), protocolCombo);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // Save settings
    }
}

void MenuConnection::onNewConnection()
{
    QDialog dialog;
    dialog.setWindowTitle(tr("New Connection"));
    dialog.setMinimumWidth(300);

    QFormLayout *layout = new QFormLayout(&dialog);

    QLineEdit *nameEdit = new QLineEdit(&dialog);
    layout->addRow(tr("Connection Name:"), nameEdit);

    QLineEdit *hostEdit = new QLineEdit(&dialog);
    layout->addRow(tr("Host/IP:"), hostEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QMessageBox::information(nullptr, tr("Success"), tr("New connection profile created."));
    }
}

void MenuConnection::onEditConnection()
{
    // Reuse New Connection dialog logic for editing
    QDialog dialog;
    dialog.setWindowTitle(tr("Edit Connection"));
    dialog.setMinimumWidth(300);

    QFormLayout *layout = new QFormLayout(&dialog);

    QComboBox *profileCombo = new QComboBox(&dialog);
    profileCombo->addItems({tr("Default Profile"), tr("Local Server")});
    layout->addRow(tr("Select Profile:"), profileCombo);

    QLineEdit *hostEdit = new QLineEdit("127.0.0.1", &dialog);
    layout->addRow(tr("Host/IP:"), hostEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        QMessageBox::information(nullptr, tr("Success"), tr("Connection profile updated."));
    }
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


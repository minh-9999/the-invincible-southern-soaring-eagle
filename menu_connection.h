#ifndef MENU_CONNECTION_H
#define MENU_CONNECTION_H


#include <QObject>
#include <QMainWindow>

class MenuConnection : public QObject
{
    Q_OBJECT

public:
    explicit MenuConnection(QObject* parent = nullptr);
    void connectionMenus(QMainWindow* window);

    void addConnectActionsToToolBar(QToolBar *toolBar, QWidget *parent);

private slots:
    void onConnectionSettings();
    void onNewConnection();
    void onEditConnection();
    void onConnectTo();
    void onShowDebugInfo();
    void onManualMode();
    void onTransmitToGame();
    void onAutoClick();
};

#endif // MENU_CONNECTION_H

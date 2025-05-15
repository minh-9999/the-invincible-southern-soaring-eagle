#ifndef MENU_SETTINGS_H
#define MENU_SETTINGS_H

#include "clockwidget.h"

#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QIcon>
#include <QObject>
#include <QMainWindow>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QLabel>

class ChessBoardWidget;
class BackgroundWidget;

class MenuSettings : public QObject
{
    Q_OBJECT


public:
    explicit MenuSettings(ChessBoardWidget *chessBoard, BackgroundWidget *backgroundWidget, ClockWidget* clock);

    void  settingsMenus(QMainWindow* window, ChessBoardWidget* boardWidget, BackgroundWidget *backgroundWidget);

    void setupClockGeometry();

private:
    ChessBoardWidget *m_chessBoard;
    BackgroundWidget *m_backgroundWidget;

    bool enableSound;
    bool showClock;
    bool winTop;

    QMap<QString, QMediaPlayer*> mediaPlayers;
    QMap<QString, QSoundEffect*> soundEffects;
    // QMap<QString, QObject*> soundEffects;

    ClockWidget* clockWidget;

    // Action handlers
    void onToggleArrow(bool checked);
    void onToggleCoordinate(bool checked);
    void onChangePieceSize(int size);
    void onToggleBackground(bool checked);
    void onChangeBackground();

public slots:
    void setEnableSound(bool enabled);
    void playSound(const QString& soundName);

    void setShowClock(bool enabled);
    void setWindowTopmost(bool enabled);
    void showOtherSettings();

};

#endif // MENU_SETTINGS_H

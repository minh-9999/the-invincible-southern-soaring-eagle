#include "menu_settings.h"
#include "ChessBoardWidget.h"
#include "BackgroundWidget.h"
#include "menu_utils.h"

#include <QFileDialog>
#include <QDir>
#include <QInputDialog>
#include <QMenuBar>

#include <QDebug>
#include <QCoreApplication>
#include <QTimer>
#include <QMessageBox>


MenuSettings::MenuSettings(ChessBoardWidget *chessBoard, BackgroundWidget *backgroundWidget, ClockWidget* clock)
    :  QObject(), m_chessBoard(chessBoard), m_backgroundWidget(backgroundWidget)
{
    // clockWidget = new ClockWidget(m_backgroundWidget); // put clock widget in background
    if (clock)
    {
        clockWidget = clock;
        clockWidget->hide(); // default hidden
    }
    else
    {
        QMessageBox::warning(nullptr,
                             tr("Warning"),
                             tr("⚠️ Warning: ClockWidget is null in MenuSettings!"));
    }

}

void MenuSettings::settingsMenus(QMainWindow* window, ChessBoardWidget* boardWidget, BackgroundWidget *backgroundWidget)
{
    QMenu *menuSettings = new QMenu("Settings", window);

    // ✅ Reuse actions from boardWidget
    menuSettings->addAction(boardWidget->toggleArrowAct);
    menuSettings->addAction(boardWidget->toggleCoordinateAct);

    QAction *toggleSoundAct = menuSettings->addAction(tr("Enable Sound"));
    toggleSoundAct->setCheckable(true);
    toggleSoundAct->setChecked(enableSound);

    QAction *toggleClockAct = menuSettings->addAction(tr("Show Clock"));
    toggleClockAct->setCheckable(true);
    toggleClockAct->setChecked(showClock);

    QAction *toggleWinTopAct = menuSettings->addAction(tr("Window Topmost"));
    toggleWinTopAct->setCheckable(true);
    toggleWinTopAct->setChecked(winTop);

    // menuSettings->addSeparator();
    MenuUtils::addFullWidthSeparator(menuSettings, 2);

    // ✅ Reuse menu from boardWidget
    menuSettings->addMenu(boardWidget->pieceSizeMenu);

    // menuSettings->addSeparator();
    MenuUtils::addFullWidthSeparator(menuSettings, 2);

    // ✅ Reuse actions from BackgroundWidget
    menuSettings->addAction(backgroundWidget->toggleBackgroundAction);
    menuSettings->addAction(backgroundWidget->changeBackgroundAction);

    QAction *otherSettings  = menuSettings->addAction(QIcon(":/icons/other_settings_icon.png"), tr("Other Settings"));
    otherSettings ->setShortcut(QKeySequence("Ctrl+R"));

    window->menuBar()->addMenu(menuSettings);

    connect(toggleSoundAct, &QAction::toggled, this, &MenuSettings::setEnableSound);
    connect(toggleClockAct, &QAction::toggled, this, &MenuSettings::setShowClock);
    connect(toggleWinTopAct, &QAction::toggled, this, &MenuSettings::setWindowTopmost);
    connect(otherSettings, &QAction::triggered, this, &MenuSettings::showOtherSettings);

}


void MenuSettings::setupClockGeometry()
{
    if (!clockWidget || !m_chessBoard)
        return;

    QRect boardRect = m_chessBoard->getBoardDisplayRect();

    int marginTop = 10;
    int clockHeight = 60;

    // Clock will be under the board
    int clockX = boardRect.left();
    int clockY = boardRect.bottom() + marginTop;
    int clockWidth = boardRect.width();

    clockWidget->setGeometry(clockX, clockY, clockWidth, clockHeight);
}


void MenuSettings::setEnableSound(bool enabled)
{
    enableSound = enabled;
    QMessageBox::information(nullptr,
                             tr("Settings"),
                             tr("Sound has been %1.").arg(enabled ? tr("enabled") : tr("disabled")));

    if (!enabled)
    {
        qDeleteAll(soundEffects);
        soundEffects.clear();
        return;
    }

    QString soundDirPath = QCoreApplication::applicationDirPath() + "/sound";
    QDir soundDir(soundDirPath);

    if (!soundDir.exists())
    {
        QMessageBox::warning(nullptr,
                             tr("Sound Error"),
                             tr("Sound directory does not exist:\n%1").arg(soundDirPath));
        return;
    }

    QStringList soundFiles = soundDir.entryList(QStringList() << "*.wav", QDir::Files);

    for (int i = 0; i < soundFiles.size(); ++i)
    {
        const QString &fileName = soundFiles.at(i);

        QString soundName = fileName.section('.', 0, 0);
        QString fullPath = soundDirPath + "/" + fileName;

        QSoundEffect* effect = new QSoundEffect(this);
        effect->setSource(QUrl::fromLocalFile(fullPath));
        // effect->setVolume(0.9);

        if (effect->status() == QSoundEffect::Error)
        {
            qWarning() << "[Sound] QSoundEffect cannot decode" << fileName << ", fallback to QMediaPlayer";
            delete effect;

            QMediaPlayer* mPlayer = new QMediaPlayer(this);
            mPlayer->setSource(QUrl::fromLocalFile(fullPath));
            // soundEffects[soundName] = mediaPlayer;
            mediaPlayers[soundName] = mPlayer;
        }
        else
        {
            soundEffects[soundName] = effect;
            qDebug() << "[Sound] Loaded:" << soundName << "from" << fileName;
        }
    }

    // ✅ Play "begin.wav" as soon as the sound is turned on
    if (soundEffects.contains("begin"))
    {
        soundEffects["begin"]->play();
        qDebug() << "[Sound] Played: begin";
    }
    else
    {
        qWarning() << "[Sound] begin.wav not loaded!";
    }
}

void MenuSettings::playSound(const QString& soundName)
{
    if (!enableSound)
        return;

    if (soundEffects.contains(soundName))
    {
        soundEffects[soundName]->play();
        qDebug() << "[Sound] Played (QSoundEffect):" << soundName;
    }
    else if (mediaPlayers.contains(soundName))
    {
        mediaPlayers[soundName]->play();
        qDebug() << "[Sound] Played (QMediaPlayer):" << soundName;
    }
    else
    {
        // qWarning() << "[Sound] Not found:" << soundName;
        QMessageBox::warning(nullptr,
                             tr("Sound Not Found"),
                             tr("Sound not found: %1").arg(soundName));
    }
}


void MenuSettings::setShowClock(bool enabled)
{
    showClock = enabled;
    qDebug() << "[Settings] Clock" << (enabled ? "shown" : "hidden");

    enabled ? clockWidget->show() : clockWidget->hide();
}

void MenuSettings::setWindowTopmost(bool enabled)
{
    // qDebug() << "[Settings] Window topmost" << (enabled ? "enabled" : "disabled");
    QMessageBox::information(nullptr, tr("Window Topmost"), tr("Window topmost %1").arg(enabled?tr("enabled") : tr("disabled")));

    auto mainWindow = qobject_cast<QMainWindow *>(parent());
    if (!mainWindow)
        return;

    Qt::WindowFlags flags = mainWindow->windowFlags();
    if (enabled)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags &= ~Qt::WindowStaysOnTopHint;

    mainWindow->setWindowFlags(flags);
    mainWindow->show(); // Call back required to apply
}

void MenuSettings::showOtherSettings()
{
    qDebug() << "[Settings] Showing other settings dialog...";
    // TODO: Open the advanced settings dialog, if available.
}

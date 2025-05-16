#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ChessBoardWidget.h"
#include "BackgroundWidget.h"
#include "game_state.h"
#include "menu_engine.h"
#include "menu_settings.h"
#include "menu_view.h"

#include <memory>

#include <QResizeEvent>
#include <QLabel>
#include <QTranslator>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE


class ResizeHandler : public QObject {
    Q_OBJECT

public:
    ResizeHandler(const QPixmap &pixmap, QLabel *label)
        : originalPixmap(pixmap), imageLabel(label) {}

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::Resize) {
            auto *resizeEvent = static_cast<QResizeEvent *>(event);
            QSize targetSize = resizeEvent->size() - QSize(40, 100);
            QPixmap scaled = originalPixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            imageLabel->setPixmap(scaled);
        }
        return QObject::eventFilter(watched, event);
    }

private:
    QPixmap originalPixmap;
    QLabel *imageLabel;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ChessBoardWidget* chessBoard;
    BackgroundWidget* background;
    ClockWidget *clockWidget;

    QString currentFileName;

    GameState game;
    QTimer* playbackTimer;

    MenuEngine* menuEngine;
    MenuSettings *menuSettings;

    QTranslator translator;
    QString currentLangCode;

    void retranslateUi();

    void resizeEvent(QResizeEvent *event);

private slots:

    // void showContextMenu(const QPoint &pos);
    void setPieceSize(ChessBoardWidget::PieceSize size);

    // File operations
    void newGame();
    void openGame();
    void saveGame();
    void saveGameAs();
    // Game info
    void showGameProperties();

    // Position operations
    void editPosition();
    void reverseBoard();
    void copyPosition();
    void copyMoveList();
    void pastePosition();

    // Help operations
    void Help();
    void About();



public slots:
    void goToFirst();
    void goToLast();
    void stepBack();
    void stepForward();
    void togglePlayback();
    void undoMove();

    void updateBoardUI();


    // Language operations
    void loadLanguage(const QString& langCode);
};
#endif // MAINWINDOW_H

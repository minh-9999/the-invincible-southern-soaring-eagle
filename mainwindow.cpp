#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "menu_file.h"

#include "menu_position.h"
#include "menu_move.h"
#include "menu_book.h"
#include "menu_connection.h"
#include "menu_settings.h"
#include "menu_view.h"
#include "menu_help.h"
#include "menu_language.h"
#include "CustomToolBar.h"
#include "CustomToolBar_2.h"

#include "clockwidget.h"


#include <QFileDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QClipboard>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>

#include <QPixmap>
#include <QDialog>
#include <QPushButton>
#include <QScrollArea>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("  The Invincible Southern Soaring Eagle  ");

    background = new BackgroundWidget(this);
    setCentralWidget(background);

    chessBoard = new ChessBoardWidget(background); // parent is background
    chessBoard->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    // chessBoard->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // chessBoard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    QVBoxLayout* layout = new QVBoxLayout(background);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(chessBoard, 0, Qt::AlignTop | Qt::AlignLeft);
    // layout->addWidget(chessBoard, 1, Qt::AlignTop | Qt::AlignLeft);


    clockWidget = new ClockWidget(background);
    clockWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    layout->addWidget(clockWidget, 0, Qt::AlignLeft);

    fileMenus(this);
    positionMenus(this);
    moveMenus(this);

    menuEngine = new MenuEngine(chessBoard);
    menuEngine->engineMenus(this);

    MenuBook* bookMenu = new MenuBook(this);
    bookMenu->bookMenus(this);

    MenuConnection* connMenu = new MenuConnection(this);
    connMenu->connectionMenus(this);

    menuSettings = new MenuSettings(chessBoard, background, clockWidget);
    menuSettings->settingsMenus(this, chessBoard, background);
    menuSettings->setupClockGeometry();

    // auto viewMenu = std::make_unique<MenuView>(this);
    // viewMen->viewMenus(this);

    MenuView *menuView = new MenuView(this);
    menuView->viewMenus(this);

    helpMenus(this);

    LanguageMenu* langMenu = new LanguageMenu(this);
    langMenu->languageMenus(this);

    connect(langMenu, &LanguageMenu::languageChanged, this, &MainWindow::loadLanguage);

    setStyleSheet(R"(QToolBar { background-color: #f0f0f0; border-top: 1px solid gray;
                        border-bottom: 1px solid gray; padding: 4px;})");


    // Toolbar
    // CustomToolBar *mainToolBar = new CustomToolBar(this);
    CustomToolBar *mainToolBar = new CustomToolBar(this, chessBoard);
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    addToolBarBreak(Qt::TopToolBarArea);

    CustomToolBar_2 *connectionToolBar= new CustomToolBar_2(mainToolBar, this);
    addToolBar(Qt::TopToolBarArea, connectionToolBar);

    menuView->bindToolBars(mainToolBar, connectionToolBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setPieceSize(ChessBoardWidget::PieceSize size)
{
    chessBoard->setPieceSize(size);  // Call function in ChessBoardWidget to change chess piece size
}


// -------------------------------------- File operations --------------------------------------
void MainWindow::newGame()
{
    chessBoard->resetBoard();
}

void MainWindow::openGame()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open Game"), // dialog title
        "",
        tr("Game Files (*.pgn *.che *.xqf *.mxq);;All Files (*)") // file filter
    );

    if (!fileName.isEmpty()) {
        chessBoard->loadFromFile(fileName);
    }
}

void MainWindow::saveGame()
{
    if (currentFileName.isEmpty())
    {
        saveGameAs();
        return;
    }

    // Save the file in PGN or XQF format depending on the file extension.
    if (currentFileName.endsWith(".pgn"))
    {
        chessBoard->saveAsPGN(currentFileName);
    }
    else if (currentFileName.endsWith(".xqf"))
    {
        chessBoard->saveAsXQF(currentFileName);
    }
    else
    {
        QMessageBox::warning(this, tr("Save Error"), tr("Unsupported file format!"));
    }
}


void MainWindow::saveGameAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Game As"), // dialog title
        "",
        tr("XQF Files (*.xqf)") // format filter
    );

    if (!fileName.isEmpty())
    {
        currentFileName = fileName;
        chessBoard->saveAsXQF(currentFileName);  // Save chess game in XQF format
    }
}

void MainWindow::showGameProperties()
{
    QMessageBox::information(this,
                             tr("Game Properties"),
                             tr("Game information will be displayed here."));
}


// -------------------------------------- Position operations --------------------------------------

void MainWindow::editPosition()
{
    QMessageBox::information(this, tr("Edit Position"), tr("Edit chess position."));
}


void MainWindow::reverseBoard()
{
    chessBoard->reverseBoard();
    QMessageBox::information(this, "Reverse Position", "The chess position has been rotated.");
}

void MainWindow::copyPosition()
{
    QString positionData = chessBoard->getPositionData();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(positionData);
    QMessageBox::information(this, tr("Copy Position"), tr("Chess position copied."));
}

void MainWindow::copyMoveList()
{
    QString moveList = chessBoard->getMoveList();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(moveList);
    QMessageBox::information(this, tr("Copy MoveList"), tr("List of moves copied."));
}

void MainWindow::pastePosition()
{
    QClipboard *clipboard = QApplication::clipboard();
    QString clipboardData = clipboard->text();

    if (chessBoard->isValidPositionData(clipboardData))
    {
        chessBoard->setPositionData(clipboardData);
        QMessageBox::information(this, tr("Paste Position"), tr("Chess position has been pasted."));
    }
    else if (chessBoard->isValidMoveList(clipboardData))
    {
        chessBoard->setMoveList(clipboardData);
        QMessageBox::information(this, tr("Paste MoveList"), tr("The list of moves has been pasted."));
    }
    else
    {
        QMessageBox::warning(this, tr("Invalid Data"), tr("Invalid paste data."));
    }
}


// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

void MainWindow::goToFirst()
{
    game.goToFirst();
    updateBoardUI();
}

void MainWindow::goToLast()
{
    game.goToLast();
    updateBoardUI();
}

void MainWindow::stepBack()
{
    game.stepBack();
    updateBoardUI();
}

void MainWindow::stepForward()
{
    game.stepForward();
    updateBoardUI();
}

void MainWindow::togglePlayback()
{
    if (playbackTimer->isActive())
        playbackTimer->stop();
    else
        playbackTimer->start(800);
}

void MainWindow::undoMove()
{
    game.undoLastMove();
    updateBoardUI();
}

void MainWindow::updateBoardUI()
{
    chessBoard->resetBoard(); // Reset to initial position (serves for Apply from 0)

    const QVector<Move>& moves = game.getMoveList();
    int limit = game.currentIndex(); // Only apply to current

    for (int i = 0; i < limit; ++i)
    {
        chessBoard->applyMove(moves[i]);
    }

    chessBoard->update();
}

void MainWindow::Help()
{
    // Open PDF document or navigate to website
    QUrl url("file:///path/to/your/document.pdf"); // Path to PDF file

    if (!QDesktopServices::openUrl(url))
    {
        // If PDF file fails, try opening website
        url.setUrl("https://www.yourwebsite.com/help");
        QDesktopServices::openUrl(url);
    }
}

void MainWindow::About()
{
    // Create About dialog with version information and image
    QDialog *aboutDialog = new QDialog(this, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint);
    aboutDialog->setWindowTitle(tr("About"));
    aboutDialog->resize(800, 600);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *versionLabel = new QLabel("Version: 1.0.0", aboutDialog);
    layout->addWidget(versionLabel);

    // Load image
    QPixmap pixmap(":/resources/eagle.jpg");

    if (pixmap.isNull())
    {
        QLabel *errorLabel = new QLabel("Unable to load image from resource.");
        layout->addWidget(errorLabel);
    }
    else
    {
        QLabel *imageLabel = new QLabel(aboutDialog);
        imageLabel->setAlignment(Qt::AlignCenter);
        imageLabel->setScaledContents(true); // allows images to scale according to QLabel

        // Let the image shrink automatically according to the window
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        layout->addWidget(imageLabel, 1); // stretch = 1 to take up maximum

        // Set initial pixmap
        QSize initialSize = aboutDialog->size() - QSize(40, 100);
        imageLabel->setPixmap(pixmap.scaled(initialSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // Set event filter to automatically resize image
        ResizeHandler *handler = new ResizeHandler(pixmap, imageLabel);
        aboutDialog->installEventFilter(handler);  // Ensure handler will receive events
    }

    aboutDialog->setLayout(layout);
    aboutDialog->exec(); // Display dialog
}

void MainWindow::loadLanguage(const QString& langCode)
{
    static QTranslator translator;

    qApp->removeTranslator(&translator);

    QString qmPath = ":/i18n/translations/chess_" + langCode + ".qm";

    if (!translator.load(qmPath))
    {
        // Fallback gets the language part (eg: vi from vi_VN)
        QString langOnly = langCode.section('_', 0, 0);
        qmPath = ":/i18n/translations/chess_" + langOnly + ".qm";

        if (!translator.load(qmPath))
        {
            QMessageBox::warning(nullptr,
                                 tr("Language Load Failed"),
                                 tr("Failed to load translation:\n%1").arg(qmPath));
            return;
        }
    }

    qApp->installTranslator(&translator);
    QMessageBox::information(nullptr,
                             tr("Language Change"),
                             tr("Language changed to: %1").arg(langCode));

    this->retranslateUi();
}



void MainWindow::retranslateUi()
{

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (menuSettings)
        menuSettings->setupClockGeometry();
}












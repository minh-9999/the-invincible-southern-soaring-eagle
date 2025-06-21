#include "ChessBoardWidget.h"
#include "PieceLoader.h"
#include "menu_utils.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QMenu>

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDateTime>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>


ChessBoardWidget::ChessBoardWidget(QWidget *parent, BoardSize size)
    : QWidget(parent), boardSize(size)
{
    baseImagePath = QCoreApplication::applicationDirPath() + "/img";
    setBoardSize(size);

    // QString defaultBoardPath = baseImagePath + "/middle/board.bmp";
    // boardImage = QPixmap(defaultBoardPath);

    // if (boardImage.isNull())
    // {
    //     qWarning() << "Failed to load board image at" << defaultBoardPath;
    // }

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &ChessBoardWidget::showContextMenu);

    updateSizeFromBoard();
    resetBoard();

    pieceSizeMenu = new QMenu(tr("Piece Size"), this);
    pieceSizeMenu->setIcon(QIcon(":/icons/size_icon.png"));

    smallPieceAct  = pieceSizeMenu->addAction(tr("Small"));
    mediumPieceAct = pieceSizeMenu->addAction(tr("Medium"));
    largePieceAct  = pieceSizeMenu->addAction(tr("Large"));

    // Connect actions
    connect(smallPieceAct, &QAction::triggered, this, [this]() {
        setPieceSize(PieceSize::Small);
    });
    connect(mediumPieceAct, &QAction::triggered, this, [this]() {
        setPieceSize(PieceSize::Medium);
    });
    connect(largePieceAct, &QAction::triggered, this, [this]() {
        setPieceSize(PieceSize::Large);
    });

    copyPositionAct = new QAction(tr("Copy Position"), this);
    copyMoveListAct = new QAction(tr("Copy Move List"), this);
    pastePositionAct = new QAction(tr("Paste Position"), this);

    connect(copyPositionAct, &QAction::triggered, this, &ChessBoardWidget::onCopyPosition);
    connect(copyMoveListAct, &QAction::triggered, this, &ChessBoardWidget::onCopyMoveList);
    connect(pastePositionAct, &QAction::triggered, this, &ChessBoardWidget::onPastePosition);

    actCopyPosition = new QAction(QIcon(":/icons/copy_icon.png"), tr("Copy Position"), this);
    actCopyPosition->setShortcut(QKeySequence("Ctrl+C"));
    addAction(actCopyPosition);
    connect(actCopyPosition, &QAction::triggered, this, &::ChessBoardWidget::requestEditPosition);

    actCopyMoveList = new QAction(QIcon(":/icons/copy_moves_icon.png"), tr("Copy Move List"), this);
    actCopyMoveList->setShortcut(QKeySequence("Ctrl+Shift+C"));
    addAction(actCopyMoveList);
    connect(actCopyMoveList, &QAction::triggered, this, &ChessBoardWidget::copyMoveListRequested);

    actPastePosition = new QAction(QIcon(":/icons/paste_icon.png"), tr("Paste Position"), this);
    actPastePosition->setShortcut(QKeySequence("Ctrl+V"));
    addAction(actPastePosition);
    connect(actPastePosition, &QAction::triggered, this, &ChessBoardWidget::pastePositionRequested);

    actAnalyzeMode = new QAction(QIcon(":/icons/analyze_icon.png"), tr("Analyze Mode"), this);
    actAnalyzeMode->setShortcut(QKeySequence("Ctrl+G"));
    addAction(actAnalyzeMode);
    connect(actAnalyzeMode, &QAction::triggered, this, &ChessBoardWidget::requestAnalyzeMode);

    actChangeMove = new QAction(QIcon(":/icons/change_moves_icon.png"), tr("Change Move"), this);
    actChangeMove->setShortcut(QKeySequence("Ctrl+J"));
    addAction(actChangeMove);
    connect(actChangeMove, &QAction::triggered, this, &ChessBoardWidget::requestChangeMove);

    actMoveNow = new QAction(QIcon(":/icons/moves_now_icon.png"), tr("Move Now"), this);
    actMoveNow->setShortcut(QKeySequence("Ctrl+M"));
    addAction(actMoveNow);
    connect(actMoveNow, &QAction::triggered, this, &ChessBoardWidget::requestMoveNow);

    actEditPosition = new QAction(QIcon(":/icons/edit_icon.png"), tr("Edit Position"), this);
    actEditPosition->setShortcut(QKeySequence("Ctrl+E"));
    addAction(actEditPosition);
    connect(actEditPosition, &QAction::triggered, this, &ChessBoardWidget::requestEditPosition);

    // for showContextMenu function
    toggleArrowAct = new QAction(tr("Show Move Arrow"), this);
    toggleArrowAct->setCheckable(true);
    toggleArrowAct->setChecked(showArrow);
    connect(toggleArrowAct, &QAction::toggled, this, &ChessBoardWidget::onToggleArrow);

    // for showContextMenu function
    toggleCoordinateAct = new QAction(tr("Show Coordinate"), this);
    toggleCoordinateAct->setCheckable(true);
    toggleCoordinateAct->setChecked(showCoordinate);
    connect(toggleCoordinateAct, &QAction::toggled, this, &ChessBoardWidget::onToggleCoordinate);

}


void ChessBoardWidget::loadBoardImage(BoardSize size)
{
    QString folder;

    switch (size)
    {
        case BoardSize::Small:  folder = "small"; break;
        case BoardSize::Medium: folder = "middle"; break;
        case BoardSize::Large:  folder = "large"; break;
        case BoardSize::Mini:   folder = "mini"; break;
    }

    QString fullPath = baseImagePath + "/" + folder + "/board.bmp";
    boardImage = QPixmap(fullPath);

    if (boardImage.isNull())
    {
        qWarning() << "Failed to load board image:" << fullPath;
    }
}

QPixmap ChessBoardWidget::removeBorder(const QPixmap &pixmap)
{
    // Convert QPixmap to QImage for easy manipulation
    QImage image = pixmap.toImage();

    int top = 0, bottom = image.height() - 1, left = 0, right = image.width() - 1;
    const int minAlpha = 10; // Only crop if alpha is lower than this level

    // Crop top and bottom borders
    for (int y = 0; y < image.height(); ++y)
    {
        for (int x = 0; x < image.width(); ++x)
        {
            if (image.pixelColor(x, y).alpha() > minAlpha)
            {
                top = std::min(top, y);
                bottom = std::max(bottom, y);
            }
        }
    }

    // Cut the left and right borders
    for (int x = 0; x < image.width(); ++x)
    {
        for (int y = 0; y < image.height(); ++y)
        {
            if (image.pixelColor(x, y).alpha() > minAlpha)
            {
                left = std::min(left, x);
                right = std::max(right, x);
            }
        }
    }

    // Crop the image
    image = image.copy(left, top, right - left + 1, bottom - top + 1);

    return QPixmap::fromImage(image);  // Convert back to QPixmap
}

QPixmap ChessBoardWidget::autoRemoveBorder(const QPixmap& pixmap, int tolerance = 20)
{
    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
    QSize size = image.size();

    // Get 4 colors from 4 corners of the image
    QList<QColor> borderColors = {
        image.pixelColor(0, 0),
        image.pixelColor(size.width() - 1, 0),
        image.pixelColor(0, size.height() - 1),
        image.pixelColor(size.width() - 1, size.height() - 1)
    };

    for (int y = 0; y < size.height(); ++y)
    {
        for (int x = 0; x < size.width(); ++x)
        {
            QColor pixelColor = image.pixelColor(x, y);

            for (const QColor& borderColor : borderColors)
            {
                int dr = abs(pixelColor.red()   - borderColor.red());
                int dg = abs(pixelColor.green() - borderColor.green());
                int db = abs(pixelColor.blue()  - borderColor.blue());

                if (dr <= tolerance && dg <= tolerance && db <= tolerance)
                {
                    image.setPixelColor(x, y, QColor(0, 0, 0, 0)); // Transparent
                    break;
                }
            }
        }
    }

    return QPixmap::fromImage(image);
}

QPixmap ChessBoardWidget::addGlowEffect(const QPixmap &src, QColor glowColor = Qt::white)
{
    const int glowSize = 6; // thickness of glow border
    QSize newSize = src.size() + QSize(glowSize * 2, glowSize * 2);

    // Create a new image larger to accommodate the border
    QImage glowImage(newSize, QImage::Format_ARGB32_Premultiplied);
    glowImage.fill(Qt::transparent);

    // Create a painter to draw on the glowImage
    QPainter painter(&glowImage);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Create a mask from the alpha of the chess piece
    QImage alphaMask = src.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < alphaMask.height(); ++y)
    {
        QRgb *line = reinterpret_cast<QRgb*>(alphaMask.scanLine(y));
        for (int x = 0; x < alphaMask.width(); ++x)
        {
            int alpha = qAlpha(line[x]);
            line[x] = qRgba(glowColor.red(), glowColor.green(), glowColor.blue(), alpha);
        }
    }

    // Create blurred edges by drawing multiple offset layers
    for (int dx = -glowSize; dx <= glowSize; ++dx)
    {
        for (int dy = -glowSize; dy <= glowSize; ++dy)
        {
            if (dx*dx + dy*dy <= glowSize*glowSize) // to make rounded edges
            {
                painter.drawImage(glowSize + dx, glowSize + dy, alphaMask);
            }
        }
    }

    // Redraw the chess piece on top
    painter.drawPixmap(glowSize, glowSize, src);
    return QPixmap::fromImage(glowImage);
}

QPixmap ChessBoardWidget::addDropShadowEffect(const QPixmap &src)
{
    const int offsetX = 3; // Offset right
    const int offsetY = 3; // Offset down
    const int blurRadius = 2; // Shadow spread
    QColor shadowColor(0, 0, 0, 120); // Shadow color black, moderate transparency

    QSize newSize = src.size() + QSize(offsetX + blurRadius, offsetY + blurRadius);
    QImage result(newSize, QImage::Format_ARGB32_Premultiplied);
    result.fill(Qt::transparent);

    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Create shadow mask from alpha
    QImage shadow = src.toImage().convertToFormat(QImage::Format_ARGB32);
    for (int y = 0; y < shadow.height(); ++y)
    {
        QRgb* line = reinterpret_cast<QRgb*>(shadow.scanLine(y));
        for (int x = 0; x < shadow.width(); ++x)
        {
            int alpha = qAlpha(line[x]);
            line[x] = qRgba(shadowColor.red(), shadowColor.green(), shadowColor.blue(), alpha);
        }
    }

    // Draw shadows by drawing multiple layers of blur to simulate a slight blur
    for (int dx = -blurRadius; dx <= blurRadius; ++dx)
    {
        for (int dy = -blurRadius; dy <= blurRadius; ++dy)
        {
            if (dx*dx + dy*dy <= blurRadius*blurRadius)
            {
                painter.drawImage(offsetX + dx, offsetY + dy, shadow);
            }
        }
    }

    // Redraw the chess piece on top
    painter.drawPixmap(0, 0, src);

    return QPixmap::fromImage(result);
}


void ChessBoardWidget::paintEvent(QPaintEvent *event)
{
    QPoint boardOffset(20, 20);
    QPoint pieceOffset(10, 10);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Get the size of the board from the enum BoardSize
    // QSize size = this->size();
    QSize size = QSize(width() - boardOffset.x(), height() - boardOffset.y());

    // Scale the board to the selected size
    QPixmap scaledBoard = boardImage.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Draw the board
    // painter.drawPixmap(0, 0, scaledBoard);
    painter.drawPixmap(boardOffset, scaledBoard);

    // Calculate the size of the squares
    int cols = 9, rows = 10;
    int cellWidth = width() / cols;
    int cellHeight = height() / rows;
    int cellSize = std::min(cellWidth, cellHeight);

    // Use cellSize for the pieces
    int pieceSize = cellSize; // Pieces can occupy the same area as the squares

    // Draw pieces from the array board[row][col]
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            const Piece &piece = board[row][col];

            if (piece.type == PieceType::None)
                continue;

            // QPoint center = cellToPixel(row, col);
            // Cell center position after offset and scale
            QPoint center = cellToPixel(row, col) + pieceOffset;

            QPixmap pixmap = getPiecePixmap(piece);

            if (pixmap.isNull()) continue;

            // pixmap = removeBorder(pixmap); // Remove the shadow border
            pixmap = autoRemoveBorder(pixmap);
            // pixmap = addGlowEffect(pixmap, Qt::white); // or QColor(255, 255, 255, 180) if you want a semi-transparent border
            // pixmap = addDropShadowEffect(pixmap);

            // Use the calculated cell size for the piece
            QPixmap scaled = pixmap.scaled(pieceSize, pieceSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            int imgW = scaled.width();
            int imgH = scaled.height();
            painter.drawPixmap(center.x() - imgW / 2,
                               center.y() - imgH / 2,
                               scaled);
        }
    }
}


void ChessBoardWidget::loadPieceImages()
{
    piecePixmaps.clear();
    QString folder;

    // qDebug() << "\n\n baseImagePath = " << baseImagePath<< "\n\n";

    switch (pieceSize)
    {
        case PieceSize::Small:  folder =  baseImagePath + "/small/";  break;
        case PieceSize::Medium: folder =  baseImagePath + "/middle/"; break;
        case PieceSize::Large:  folder =  baseImagePath + "/large/";  break;
    }

    // Iterate through piece types and colors
    // Piece type examples: r, a, b, c, p, k, n
    QStringList colors = { "r", "b" };
    // Piece color: red (r), black (b)
    QStringList types  = { "a", "b", "c", "k", "n", "p", "r" };

    for (const auto& pieceType : types)
    {
        for (const auto& color : colors)
        {
            QString fileName = QString(color) + QString(pieceType) + ".bmp";
            QString fullPath = folder + fileName;
            // qDebug() << "\n Loading image from file:  " << fullPath;

            QPixmap pixmap(fullPath);
            piecePixmaps[QString(color) + QString(pieceType)] = pixmap;
        }
    }
}


QRect ChessBoardWidget::getCellRect(int row, int col) const
{
    int cellW = width()  / 9;
    int cellH = height() / 10;
    return QRect(col * cellW, row * cellH, cellW, cellH);
}

QPixmap ChessBoardWidget::getPiecePixmap(const Piece& piece) const
{
    if (piece.type == PieceType::None)
        return QPixmap(); // Draw nothing

    return PieceLoader::loadPiecePixmap(piece.type, piece.isRed, boardSize);
}


// ***************************************************************************************

void ChessBoardWidget::mousePressEvent(QMouseEvent *event)
{
    int cols = 9, rows = 10;
    int cellWidth = width() / cols;
    int cellHeight = height() / rows;

    QPoint pos = event->position().toPoint();
    int col = pos.x() / cellWidth;
    int row = pos.y() / cellHeight;

    if (row >= 0 && row < 10 && col >= 0 && col < 9)
    {
        selectedCell = QPoint(col, row);
        update();
    }

}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


QPoint ChessBoardWidget::cellToPixel(int row, int col) const
{
    int leftMargin = 0;
    int topMargin = 0;
    int cellSize = 0;
    int pieceSize = 0;
    int offsetX = 0;
    int offsetY = 0;

    switch(boardSize)
    {
    case BoardSize::Large:
        cellSize = 53;
        pieceSize = 46;
        leftMargin = 70;
        topMargin = 71;
        offsetX = 2;
        offsetY = 2;
        break;

    case BoardSize::Medium:
        cellSize = 40;
        pieceSize = 35;
        leftMargin = 75;
        topMargin = 68;
        offsetX = 1;
        offsetY = 1;
        break;

    case BoardSize::Small:
        cellSize = 26;
        pieceSize = 28;
        leftMargin = 40;
        topMargin = 42;
        offsetX = 1;
        offsetY = 1;
        break;

    case BoardSize::Mini:
        cellSize = 16;
        pieceSize = 18;
        leftMargin = 10;
        topMargin = 12;
        offsetX = 0;
        offsetY = 0;
        break;

    default:
        break;
    }

    int centerX = leftMargin + col * cellSize;
    int centerY = topMargin + row * cellSize;

    QPoint offset(20, 20);

    return QPoint(centerX - pieceSize / 2 + offsetX,
                  centerY - pieceSize / 2 + offsetY);

}


QRect ChessBoardWidget::getBoardDisplayRect() const
{
    QPoint boardOffset(20, 20);
    QSize boardSize(width() - boardOffset.x(), height() - boardOffset.y());

    QPixmap scaledBoard = boardImage.scaled(boardSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return QRect(boardOffset, scaledBoard.size());
}


void ChessBoardWidget::updateSizeFromBoard()
{
    int baseWidth = boardImage.width();
    int baseHeight = boardImage.height();

    // switch (currentSize)
    // {
    // case BoardSize::Small:
    //     setFixedSize(baseWidth, baseHeight);
    //     break;

    // case BoardSize::Medium:
    //     setFixedSize(baseWidth, baseHeight);
    //     break;

    // case BoardSize::Large:
    //     setFixedSize(baseWidth, baseHeight);
    //     break;

    // case BoardSize::Mini:
    //     // Mini not used in main widget, ignore resize
    //     break;

    // default:
    //     qWarning() << "Unknown board size!";
    //     break;
    // }

    qDebug() << "\n Updating widget size to:  " << baseWidth << "x" << baseHeight;

    setFixedSize(baseWidth, baseHeight);
    updateGeometry();
}

void ChessBoardWidget::setBoardSize(BoardSize size)
{
    currentSize = size;
    QString folder;

    switch (size)
    {
        case BoardSize::Small:
            folder = "small";
            break;

        case BoardSize::Medium:
            folder = "middle";
            break;

        case BoardSize::Large:
            folder = "large";
            break;

        case BoardSize::Mini:
            return; // Do not change boardImage or main board size
    }

    QString fullPath = baseImagePath + "/" + folder + "/board.bmp";
    boardImage = QPixmap(fullPath);

    // Reload the board image and update the widget size
    loadBoardImage(size);

    qDebug() << "\n  [DEBUG] boardImage size loaded =" << boardImage.size();

    updateSizeFromBoard();

    // Map board size to piece size
    PieceSize pieceSize;

    if (size == BoardSize::Small)
        pieceSize = PieceSize::Small;

    else if (size == BoardSize::Medium)
        pieceSize = PieceSize::Medium;

    else if (size == BoardSize::Large)
        pieceSize = PieceSize::Large;

    setPieceSize(pieceSize); // ✅ calling the correct function will resize + update the unit

    update(); // Call again paintEvent
}


QSize ChessBoardWidget::sizeHint() const
{
    if (boardImage.isNull())
        return QSize(400, 400); // fallback

    QSize sz = boardImage.size(); // or calculated by boardSize
    qDebug() << "[sizeHint] returning: " << sz;
    return sz;
}


// ----------------------------------------------------


void ChessBoardWidget::resetBoard()
{
    // Suppose you already have a class Piece { PieceType type; bool isRed; }
    // Initialize a 10x9 chess board
    board = QVector<QVector<Piece>>(10, QVector<Piece>(9, Piece())); // Default is None

    // Reset the pieces to their original positions (playing traditional Chinese chess)
    setupInitialPosition();

    currentMoveIndex = 0;
    update();
}

void ChessBoardWidget::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);

    menu.addMenu(pieceSizeMenu);

    // === Show options ===

    toggleArrowAct->setChecked(showArrow);
    toggleCoordinateAct->setChecked(showCoordinate);

    menu.addAction(toggleArrowAct);
    menu.addAction(toggleCoordinateAct);

    // menu.addSeparator();
    MenuUtils::addFullWidthSeparator(&menu, 2);

    menu.addAction(actCopyPosition);
    menu.addAction(actCopyMoveList);
    menu.addAction(actPastePosition);

    // menu.addSeparator();
    MenuUtils::addFullWidthSeparator(&menu, 2);

    menu.addAction(actAnalyzeMode);
    menu.addAction(actChangeMove);
    menu.addAction(actMoveNow);

    // menu.addSeparator();
    MenuUtils::addFullWidthSeparator(&menu, 2);
    menu.addAction(actEditPosition);

    menu.exec(pos);  // Show context menu

}

void ChessBoardWidget::loadFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open file:" << fileName;
        return;
    }

    QTextStream in(&file);
    moves.clear();

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        // Ignore metadata
        if (line.startsWith("title=") || line.startsWith("event=") || line.contains('=')) continue;
        if (line.isEmpty()) continue;

        // Example: "1. 炮二平五 马８进７"
        static const QRegularExpression re(R"((\d+)\.\s+(\S+)\s+(\S+))");
        QRegularExpressionMatch match = re.match(line);

        if (match.hasMatch())
        {
            QString redMove = match.captured(2);
            QString blackMove = match.captured(3);
            moves.append(redMove);
            moves.append(blackMove);
        }
    }

    currentMoveIndex = 0;
    update();
}

void ChessBoardWidget::saveAsXQF(const QString &fileName)
{
    QString xqfData = getXQFFormat();

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file for saving!";
        return;
    }

    QTextStream out(&file);
    out << xqfData;

    file.close();
}


void ChessBoardWidget::saveAsPGN(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qWarning() << "Unable to open file for saving!";
        return;
    }

    QTextStream out(&file);

    // Write meta information to PGN file
    out << "[Event \"Chua Vua\"]\n";
    out << "[Site \"Ha Noi\"]\n";
    out << "[Date \"" << QDateTime::currentDateTime().toString("yyyy.MM.dd") << "\"]\n";
    out << "[White \"Player 1\"]\n";
    out << "[Black \"Player 2\"]\n\n";

    // Write moves to PGN file
    const QList<QString> &moves = gameMoves;
    int moveCount = 1;
    for (const QString &move : moves)
    {
        out << moveCount << ". " << move << " ";
        moveCount++;
    }

    file.close();
}

QString ChessBoardWidget::getXQFFormat()
{
    QString xqfData;

    // Meta info
    const QString currentDate = QDateTime::currentDateTime().toString("yyyy.MM.dd");
    xqfData += "[Event \"Chinese Chess\"]\n"
               "[Site \"Ha Noi\"]\n"
               "[Date \"" + currentDate + "\"]\n"
                               "[Player1 \"Player 1\"]\n"
                               "[Player2 \"Player 2\"]\n\n";

    // Board
    xqfData += "[Board]\n";
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            const Piece &piece = board[row][col];
            if (piece.isEmpty())
            {
                xqfData += ". ";
                continue;
            }

            const bool isRed = piece.isRed;
            QChar symbol = '.';

            switch (piece.type) {
            case PieceType::Pawn:    symbol = isRed ? 'P' : 'p'; break;
            case PieceType::Rook:    symbol = isRed ? 'R' : 'r'; break;
            case PieceType::Knight:  symbol = isRed ? 'N' : 'n'; break;
            case PieceType::Bishop:  symbol = isRed ? 'B' : 'b'; break;
            case PieceType::Advisor: symbol = isRed ? 'A' : 'a'; break;
            case PieceType::King:    symbol = isRed ? 'K' : 'k'; break;
            case PieceType::Cannon:  symbol = isRed ? 'C' : 'c'; break;
            default: break;
            }

            xqfData += symbol;
            xqfData += ' ';
        }
        xqfData += '\n';
    }

    xqfData += "\n[Moves]\n";
    for (int i = 0; i < gameMoves.size(); ++i) {
        xqfData += gameMoves[i] + " ";
    }

    return xqfData;
}


bool ChessBoardWidget::loadPGN(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open PGN file.");
        return false;
    }

    QTextStream in(&file);
    QString pgnData = in.readAll();
    file.close();

    // TODO: Parse PGN here
    qDebug() << "[PGN] Loaded content:" << pgnData.left(200);

    // parsePGN(pgnData);
    return true;
}

bool ChessBoardWidget::loadXQF(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Failed to open XQF file.");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    // TODO: Parse XQF binary data
    qDebug() << "[XQF] Loaded binary size:" << data.size();

    // parseXQF(data);
    return true;
}

bool ChessBoardWidget::loadCHE(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open CHE file.");
        return false;
    }

    QTextStream in(&file);
    QString cheData = in.readAll();
    file.close();

    // TODO: Parse CHE format
    qDebug() << "[CHE] Loaded content:" << cheData.left(200);

    // parseCHE(cheData);
    return true;
}

bool ChessBoardWidget::loadMXQ(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error", "Failed to open MXQ file.");
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    // TODO: Parse MXQ binary or XML if it's structured
    qDebug() << "[MXQ] Loaded binary size:" << data.size();

    // parseMXQ(data);
    return true;
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

QString ChessBoardWidget::getPositionData()
{
    QStringList lines;
    lines.reserve(board.size());  // Avoid realloc

    for (int row = 0; row < board.size(); ++row) {
        const auto& rowData = board[row];
        QStringList pieces;
        pieces.reserve(rowData.size());

        for (int col = 0; col < rowData.size(); ++col)
        {
            pieces << rowData[col].toString();
        }

        lines << pieces.join(" ");
    }

    return lines.join("\n");
}


void ChessBoardWidget::setPositionData(const QString& data)
{
    QStringList rows = data.split('\n', Qt::SkipEmptyParts);

    if (rows.size() != board.size())
    {
        qWarning() << "Invalid number of rows in position data";
        return;
    }

    static const QRegularExpression spaceRegex("\\s+");

    for (int row = 0; row < board.size(); ++row) {
        QStringList cols = rows[row].split(spaceRegex, Qt::SkipEmptyParts);

        if (cols.size() != board[row].size())
        {
            qWarning() << "Invalid number of columns in row" << row;
            continue;
        }

        for (int col = 0; col < board[row].size(); ++col) {
            board[row][col].setFromString(cols[col]);
        }
    }

    update();
}



QString ChessBoardWidget::getMoveList()
{
    QString moveList;

    return moveList;
}


void ChessBoardWidget::setMoveList(const QString& moveList)
{

}


bool ChessBoardWidget::isValidPositionData(const QString& data)
{
    // Check if the data string is in the correct format
    // For example, check if the row number, column number, or if the chess piece wildcards are valid
    return true; // Or return false if the data is invalid
}


bool ChessBoardWidget::isValidMoveList(const QString& data)
{
    // Check if the move string is in valid format
    return true;
}


void ChessBoardWidget::reverseBoard()
{
    int boardHeight = board.size();
    if (boardHeight == 0) return;

    int boardWidth = board[0].size();

    for (int row = 0; row < boardHeight / 2; ++row)
    {
        for (int col = 0; col < boardWidth; ++col)
        {
            std::swap(board[row][col], board[boardHeight - row - 1][col]);
        }
    }

    update();
}



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

void ChessBoardWidget::setupInitialPosition()
{
    std::array<PieceType, 9> backRow = {
        PieceType::Rook, PieceType::Knight, PieceType::Bishop,
        PieceType::Advisor, PieceType::King, PieceType::Advisor,
        PieceType::Bishop, PieceType::Knight, PieceType::Rook
    };

    for (int col = 0; col < 9; ++col)
    {
        board[0][col] = Piece{backRow[col], false}; // Black
        board[9][col] = Piece{backRow[col], true};  // Red
    }

    board[2][1] = board[2][7] = Piece{PieceType::Cannon, false};
    board[7][1] = board[7][7] = Piece{PieceType::Cannon, true};

    for (int i = 0; i < 5; ++i)
    {
        board[3][i * 2] = Piece{PieceType::Pawn, false}; // Black pawns
        board[6][i * 2] = Piece{PieceType::Pawn, true};  // Red pawns
    }
}


void ChessBoardWidget::stepForward()
{
    if (currentMoveIndex >= moves.size())
    {
        qDebug() << "No more moves.";
        return;
    }

    QString moveText = moves[currentMoveIndex++];
    qDebug() << "Move:" << moveText;

    // TODO: Parse & execute the move on board[row][col]
    // Hint: use regex or Chinese dictionary -> coordinates

    update();
}

// Check for valid chess piece symbols
bool ChessBoardWidget::isValidPiece(const QString& piece)
{
    static const QSet<QString> validPieces = {
        // Red piece
        "車", "馬", "相", "仕", "帥", "炮", "兵",
        // Black piece
        "車", "馬", "象", "士", "將", "炮", "卒"
    };
    return validPieces.contains(piece);
}

void ChessBoardWidget::setPieceSize(PieceSize size)
{
    pieceSize = size; // Update the size of the piece

    loadPieceImages();
    update(); // Update the board interface
}

// Get the piece symbol in the cell (row, col)
QString ChessBoardWidget::getPieceSymbol(int row, int col)
{
    return board[row][col].toString(); // ✅ Return the piece symbol
}

// Swap pieces between two cells
void ChessBoardWidget::swapPieces(int row1, int col1, int row2, int col2)
{
    std::swap(board[row1][col1], board[row2][col2]); // ✅ Concise and correct
}

// ***************************************************************************************

void ChessBoardWidget::applyMove(const Move &move)
{
    board[move.toX][move.toY] = board[move.fromX][move.fromY];
    board[move.fromX][move.fromY] = Piece::empty();
    update();
}

const Piece* ChessBoardWidget::getPieceAt(int x, int y) const
{
    return &board[x][y];
}

// ***************************************************************************************


void ChessBoardWidget::requestAnalyzeMode()
{
    // Move analysis logic
    qDebug() << "Analyzing move...";

    // Perform analysis and update UI if needed
    emit onAnalysisCompleted();
}

// Move change request function
void ChessBoardWidget::requestChangeMove()
{
    // Move change logic
    qDebug() << "Changing move...";

    // Check move validity and change
    emit onMoveChanged();
}

// Function to request an immediate move
void ChessBoardWidget::requestMoveNow()
{
    // Logic to execute an immediate move
    qDebug() << "Executing an immediate move...";
    // Update the board state and UI
    emit onMovePerformed();
}

// Function to request the position of the chess piece
void ChessBoardWidget::requestEditPosition()
{
    // Logic to edit the chess position
    qDebug() << "Editing the chess position...";
    // Allow the player to change the position of the chess piece on the board
    emit onPositionEdited();
}

// ***************************************************************************************

void ChessBoardWidget::onCopyPosition()
{
    qDebug() << "Copy chess position...";
}

void ChessBoardWidget::onCopyMoveList()
{
    qDebug() << "Copy move list...";
}

void ChessBoardWidget::onPastePosition()
{
    qDebug() << "Paste chess position...";
}

// #############################################################################

void ChessBoardWidget::onAnalysisCompleted()
{
    qDebug() << "Move analysis completed!";
    // Update UI if needed
}

void ChessBoardWidget::onMoveChanged()
{
    qDebug() << "[ChessBoardWidget] Move has been changed!";

    // Can add logic to re-mark current move
    highlightCurrentMove();

    update();
}

void ChessBoardWidget::onMovePerformed()
{
    qDebug() << "[ChessBoardWidget] Move has been performed!";

    // Can update last move or log again
    saveMoveToHistory();

    update();
}

void ChessBoardWidget::onPositionEdited()
{
    qDebug() << "[ChessBoardWidget] Board position has been edited!";

    // When user manually adjusts chess position, need to reset states
    clearMoveHistory(); // clear history because it is no longer correct
    validateBoardState(); // check valid board if needed

    update();
}

void ChessBoardWidget::onToggleArrow(bool checked)
{
    showArrow = checked;
    // qDebug() << "[ChessBoardWidget] Show move arrow:" << (showArrow ? "Enabled" : "Disabled");

    update();
}

void ChessBoardWidget::onToggleCoordinate(bool checked)
{
    showCoordinate = checked;
    // qDebug() << "[ChessBoardWidget] Show coordinates:" << (showCoordinate ? "Enabled" : "Disabled");

    update();
}


// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


void ChessBoardWidget::highlightCurrentMove()
{
    // Suppose you have variables currentMoveStart and currentMoveEnd (QPoint)
    if (!currentMoveStart.isNull() && !currentMoveEnd.isNull())
    {
        qDebug() << "[ChessBoardWidget] Highlighting move from"
                 << currentMoveStart << "to" << currentMoveEnd;
    }
    else
    {
        qDebug() << "[ChessBoardWidget] No move to highlight.";
    }

}

void ChessBoardWidget::saveMoveToHistory()
{
    Move move;
    move.start = currentMoveStart;
    move.end = currentMoveEnd;
    move.movedPiece = board[currentMoveStart.x()][currentMoveStart.y()];
    move.capturedPiece = board[currentMoveEnd.x()][currentMoveEnd.y()];

    moveHistory.append(move);

    qDebug() << "[ChessBoardWidget] Move saved to history:"
             << move.start << "->" << move.end;
}


void ChessBoardWidget::clearMoveHistory()
{
    moveHistory.clear();
    qDebug() << "[ChessBoardWidget] Move history cleared.";
}

void ChessBoardWidget::validateBoardState()
{
    bool valid = true;

    int redKing = 0, blackKing = 0;

    for (int y = 0; y < 10; ++y)
    {
        for (int x = 0; x < 9; ++x)
        {
            const Piece& p = board[x][y];
            if (p.type == PieceType::King)
            {
                if (p.isRed) redKing++;
                else blackKing++;
            }
        }
    }

    if (redKing != 1 || blackKing != 1)
    {
        valid = false;
        qWarning() << "[ChessBoardWidget] Invalid board: redKing=" << redKing
                   << ", blackKing=" << blackKing;
    }

    if (valid)
    {
        qDebug() << "[ChessBoardWidget] Board state is valid.";
    }
    else
    {
        QMessageBox::warning(this, "Invalid Board",
                             "Board must contain exactly one red king and one black king.");
    }
}

// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void ChessBoardWidget::onEngineSetting()
{
    qDebug() << "Opening Engine Settings...";
    // This could be where you open a dialog for the user to select an engine or configure
}

void ChessBoardWidget::onEngineBlack()
{
    // Example: Set the engine for the black side
    currentEngineBlack = "Engine Black Example"; // Replace with the actual engine
    qDebug() << "Engine for Black side set to:" << currentEngineBlack;
    // Perform the necessary operations to change the engine for the black side
}

void ChessBoardWidget::onEngineRed()
{
    // Example: Set the engine for the red side
    currentEngineRed = "Engine Red Example"; // Replace with actual engine
    qDebug() << "Engine for Red side set to:" << currentEngineRed;
    // Perform necessary operations to change engine for red side
}


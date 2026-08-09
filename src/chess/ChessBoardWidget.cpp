#include "ChessBoardWidget.h"
#include "BackgroundWidget.h"
#include "PieceLoader.h"
#include "../menus/menu_utils.h"

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
#include <QInputDialog>
#include <algorithm>


ChessBoardWidget::ChessBoardWidget(QWidget *parent, BoardSize size)
    : QWidget(parent), boardSize(size)
{
    baseImagePath = ":/img";  // Use Qt resource path
    board.resize(10, QVector<Piece>(9));
    setBoardSize(size);

    loadPieceImages();

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            this, &ChessBoardWidget::showContextMenu);

    updateSizeFromBoard();
    resetBoard();

    pieceSizeMenu = new QMenu(tr("Board Size"), this);
    pieceSizeMenu->setIcon(QIcon(":/icons/size_icon.png"));

    smallPieceAct  = pieceSizeMenu->addAction(tr("Small"));
    mediumPieceAct = pieceSizeMenu->addAction(tr("Medium"));
    largePieceAct  = pieceSizeMenu->addAction(tr("Large"));

    // Connect actions
    connect(smallPieceAct, &QAction::triggered, this, [this]() {
        setBoardSize(BoardSize::Small);
    });
    connect(mediumPieceAct, &QAction::triggered, this, [this]() {
        setBoardSize(BoardSize::Medium);
    });
    connect(largePieceAct, &QAction::triggered, this, [this]() {
        setBoardSize(BoardSize::Large);
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

    customThemeMenu = new QMenu(tr("Custom Themes"), this);
    customThemeMenu->setIcon(QIcon(":/icons/custom_icon.png"));

    //
    actLoadCustomBoard = new QAction(QIcon(":/icons/custom_board_icon.png"), tr("Load Custom Board..."), this);
    customThemeMenu->addAction(actLoadCustomBoard);
    connect(actLoadCustomBoard, &QAction::triggered, this, [this]() {
        QString dir = QCoreApplication::applicationDirPath();
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Select Custom Board Image Directory"),
                                                        dir, tr("Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)"));
        if (!fileName.isEmpty()) {
            loadCustomBoard(fileName); // Gọi hàm xử lý đã có
        }
    });

    //
    actLoadCustomPieces = new QAction(QIcon(":/icons/custom_piece_icon.png"), tr("Load Custom Pieces..."), this);
    customThemeMenu->addAction(actLoadCustomPieces);
    connect(actLoadCustomPieces, &QAction::triggered, this, [this]() {
        QString dir = QCoreApplication::applicationDirPath();
        QString folderPath = QFileDialog::getExistingDirectory(this,
                                                               tr("Select Custom Pieces Directory"), dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!folderPath.isEmpty()) {
            loadCustomPieces(folderPath); // Gọi hàm xử lý đã có
        }
    });

    customThemeMenu->addSeparator();
    //
    actResetCustomTheme = new QAction(QIcon(":/icons/reset_icon.png"), tr("Reset to Default Theme"), this);
    customThemeMenu->addAction(actResetCustomTheme);
    connect(actResetCustomTheme, &QAction::triggered, this, &ChessBoardWidget::resetCustomTheme);

    m_pieceSetMenu = new QMenu(tr("Piece Sets"), this);
    m_pieceSetMenu->setIcon(QIcon(":/icons/pieces_icon.png"));
}

void ChessBoardWidget::loadBoardCategories()
{
    QString appDir = QCoreApplication::applicationDirPath();

    // Add "Board image co duong ke" folder
    QString boardWithLinesPath = appDir + "/Board image co duong ke";
    QDir boardWithLinesDir(boardWithLinesPath);
    if (boardWithLinesDir.exists()) {
        boardCategories["Board image co duong ke"] = boardWithLinesPath;
        qDebug() << "Found board category: Board image co duong ke at" << boardWithLinesPath;
    }

    // Add "Board suu tam" folder
    QString boardSuuTamPath = appDir + "/Board suu tam";
    QDir boardSuuTamDir(boardSuuTamPath);
    if (boardSuuTamDir.exists()) {
        boardCategories["Board suu tam"] = boardSuuTamPath;
        qDebug() << "Found board category: Board suu tam at" << boardSuuTamPath;
    }

    // Also check for subdirectories in "Board suu tam" (like "天天棋盘修复")
    if (boardSuuTamDir.exists()) {
        QStringList subDirs = boardSuuTamDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& subDir : std::as_const(subDirs)) {
            QString subDirPath = boardSuuTamDir.filePath(subDir);
            QString categoryName = "Board suu tam/" + subDir;
            boardCategories[categoryName] = subDirPath;
            qDebug() << "Found board subcategory:" << categoryName << "at" << subDirPath;
        }
    }

    // Also check for subdirectories in "Background cho quan co" for piece sets
    QString pieceSetsPath = appDir + "/Background cho quan co";
    QDir pieceSetsDir(pieceSetsPath);
    if (pieceSetsDir.exists()) {
        QStringList subDirs = pieceSetsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString& subDir : std::as_const(subDirs)) {
            QString subDirPath = pieceSetsDir.filePath(subDir);
            QString categoryName = "Pieces/" + subDir;
            boardCategories[categoryName] = subDirPath;
            pieceCategories[subDir] = subDirPath;
            qDebug() << "Found piece set:" << categoryName << "at" << subDirPath;
        }
    }
}

void ChessBoardWidget::createBoardSelectionMenu()
{
    if (boardSelectionMenu) {
        boardSelectionMenu->deleteLater();
    }

    boardSelectionMenu = new QMenu(tr("Select Board"), this);

    // Add board categories
    for (auto it = boardCategories.begin(); it != boardCategories.end(); ++it) {
        const QString& categoryName = it.key();
        const QString& categoryPath = it.value();

        QMenu* categoryMenu = boardSelectionMenu->addMenu(categoryName);

        // Get images in this category
        QDir dir(categoryPath);
        QStringList filters = {"*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif", "*.webp", "*.tiff", "*.svg"};
        QStringList images = dir.entryList(filters, QDir::Files);

        if (images.isEmpty()) {
            // Check subdirectories
            QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
            for (const QString& subDir : std::as_const(subDirs)) {
                QDir subDirObj(dir.filePath(subDir));
                QStringList subImages = subDirObj.entryList(filters, QDir::Files);
                for (const QString& img : std::as_const(subImages)) {
                    QString actionName = subDir + "/" + img;
                    QAction* action = categoryMenu->addAction(actionName);
                    connect(action, &QAction::triggered, this, [this, categoryName, subDir, img]() {
                        loadBoardFromCategory(categoryName, subDir + "/" + img);
                    });
                }
            }
        } else {
            for (const QString& img : std::as_const(images)) {
                QAction* action = categoryMenu->addAction(img);
                connect(action, &QAction::triggered, this, [this, categoryName, img]() {
                    loadBoardFromCategory(categoryName, img);
                });
            }
        }
    }

    // Add separator and reset action
    boardSelectionMenu->addSeparator();
    QAction* resetAction = boardSelectionMenu->addAction(tr("Reset to Default Board"));

    connect(resetAction, &QAction::triggered, this, [this]() {
        // Reset to default board
        QString bgFolder = baseImagePath + "/bg/";
        QString defaultBgPath = bgFolder + "bg5.jpg";
        BackgroundWidget bgw;
        bgw.backgroundImage.load(defaultBgPath);
        currentBoardCategory.clear();
        currentBoardImage.clear();
        update();
        emit boardChanged("", "");
    });
}

void ChessBoardWidget::loadBoardFromCategory(const QString& category, const QString& imageName)
{
    if (!boardCategories.contains(category)) {
        qWarning() << "Category not found:" << category;
        return;
    }

    QString categoryPath = boardCategories[category];
    QString imagePath;

    // Check if imageName contains a subdirectory
    if (imageName.contains("/")) {
        QStringList parts = imageName.split("/");
        QString subDir = parts[0];
        QString fileName = parts[1];
        imagePath = QDir(categoryPath).filePath(subDir + "/" + fileName);
    } else {
        imagePath = QDir(categoryPath).filePath(imageName);
    }

    QFileInfo fileInfo(imagePath);
    if (!fileInfo.exists()) {
        qWarning() << "Board image not found:" << imagePath;
        return;
    }

    QPixmap newBoard(imagePath);
    if (newBoard.isNull()) {
        qWarning() << "Failed to load board image:" << imagePath;
        return;
    }

    boardImage = newBoard;
    currentBoardCategory = category;
    currentBoardImage = imageName;

    // Resize widget to match board image if needed
    setFixedSize(boardImage.size());
    updateGeometry();

    update();
    emit boardChanged(category, imagePath);

    qDebug() << "Loaded board from category:" << category << "image:" << imageName;
}

void ChessBoardWidget::setBoardCategory(const QString& category)
{
    if (!boardCategories.contains(category)) {
        qWarning() << "Category not found:" << category;
        return;
    }

    currentBoardCategory = category;
    // Load first image from category
    QString categoryPath = boardCategories[category];
    QDir dir(categoryPath);
    QStringList filters = {"*.png", "*.jpg", "*.jpeg", "*.bmp", "*.gif", "*.webp", "*.tiff", "*.svg"};
    QStringList images = dir.entryList(filters, QDir::Files);

    if (!images.isEmpty()) {
        loadBoardFromCategory(category, images.first());
    }
}

void ChessBoardWidget::resetBoard()
{
    // Reset the pieces to their original positions (playing traditional Chinese chess)
    setupInitialPosition();

    currentMoveIndex = 0;
    update();
}

void ChessBoardWidget::loadBoardImage(BoardSize size)
{
    if (!customBoardPath.isEmpty())
    {
        QPixmap customBg(customBoardPath);
        if (!customBg.isNull())
        {
            // Scale the custom board to match the expected dimensions of the selected board size
            // This ensures our hardcoded grid metrics still perfectly align!
            if (size == BoardSize::Large) 
                boardImage = customBg.scaled(572, 626, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            else if (size == BoardSize::Medium) 
                boardImage = customBg.scaled(432, 473, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            else if (size == BoardSize::Small) 
                boardImage = customBg.scaled(292, 318, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            else 
                boardImage = customBg;
                
            return;
        }
        else
        {
            qWarning() << "Failed to load custom board image:" << customBoardPath;
        }
    }

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
            if (dx*dx + dy*dy <= blurRadius*blurRadius) // to make rounded edges
            {
                painter.drawImage(offsetX + dx, offsetY + dy, shadow);
            }
        }
    }

    // Redraw the chess piece on top
    painter.drawPixmap(0, 0, src);

    return QPixmap::fromImage(result);
}

void ChessBoardWidget::PieceSetMenu()
{
    if (!m_pieceSetMenu) return;

    m_pieceSetMenu->clear();

    // Thêm các action cố định
    QAction* loadPieceSetAct = m_pieceSetMenu->addAction(QIcon(":/icons/pieceset_icon.png"), tr("Load Piece Set from Folder..."));
    connect(loadPieceSetAct, &QAction::triggered, this, qOverload<>(&ChessBoardWidget::onLoadPieceSet));

    QAction* loadPieceSetFromCategoryAct = m_pieceSetMenu->addAction(QIcon(":/icons/pieceset_category_icon.png"), tr("Load Piece Set from Category..."));
    connect(loadPieceSetFromCategoryAct, &QAction::triggered, this, &ChessBoardWidget::onLoadPieceSetFromCategory);

    m_pieceSetMenu->addSeparator();

    // Quét thư mục
    const QMap<QString, QString>& categories = getBoardCategories();
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        const QString& categoryName = it.key();
        const QString& categoryPath = it.value();

        if (categoryName.startsWith("Pieces/")) {
            QString displayName = categoryName.mid(7);
            QAction* categoryAction = m_pieceSetMenu->addAction(displayName);
            connect(categoryAction, &QAction::triggered, this, [this, categoryPath]() {
                onLoadPieceSet(categoryPath);
            });
        }
    }

    m_pieceSetMenu->addSeparator();

    QAction* refreshAct = m_pieceSetMenu->addAction(QIcon(":/icons/refresh_icon.png"), tr("Refresh Piece Sets"));
    connect(refreshAct, &QAction::triggered, this, &ChessBoardWidget::PieceSetMenu);
}

void ChessBoardWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // Draw the board at actual size (since widget is sized to match board image)
    painter.drawPixmap(0, 0, boardImage);

    // Draw valid move indicators if a piece is selected
    if (selectedCell != QPoint(-1, -1) && !validMoves.isEmpty()) {
        painter.setPen(QPen(QColor(0, 255, 0, 180), 3));
        painter.setBrush(QBrush(QColor(0, 255, 0, 100)));
        
        for (const QPoint& move : std::as_const(validMoves)) {
            int col = move.x();
            int row = move.y();
            QPoint center = cellToPixel(row, col);
            int radius = qMin(calculatedCellWidth, calculatedCellHeight) / 3;
            painter.drawEllipse(center, radius, radius);
        }
    }

    // Draw pieces from the array board[row][col]
    for (int row = 0; row < 10; ++row)
    {
        for (int col = 0; col < 9; ++col)
        {
            const Piece &piece = board[row][col];

            if (piece.type == PieceType::None)
                continue;

            // Get the center position for this cell using detected grid
            QPoint center = cellToPixel(row, col);

            QPixmap pixmap = getPiecePixmap(piece);

            if (pixmap.isNull()) continue;

            // Remove border from piece image
            pixmap = autoRemoveBorder(pixmap);

            // Use the calculated piece size for scaling
            int pieceSize = calculatedPieceSize;
            if (pieceSize == 0)
            {
                // Fallback to default if not calculated
                pieceSize = qMin(calculatedCellWidth, calculatedCellHeight);
                if (pieceSize == 0) pieceSize = 35;
            }

            // Get original dimensions
            int originalWidth = pixmap.width();
            int originalHeight = pixmap.height();

            // Calculate scale factor to fit within the piece size while maintaining aspect ratio
            double scale = qMin(double(pieceSize) / originalWidth, double(pieceSize) / originalHeight);

            int scaledWidth = static_cast<int>(originalWidth * scale);
            int scaledHeight = static_cast<int>(originalHeight * scale);

            // Scale maintaining original aspect ratio
            QPixmap scaled = pixmap.scaled(scaledWidth, scaledHeight, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

            // Center the scaled image
            int imgW = scaled.width();
            int imgH = scaled.height();
            
            // Highlight selected piece
            if (selectedCell == QPoint(col, row)) {
                painter.setPen(QPen(QColor(255, 255, 0, 200), 4));
                painter.setBrush(Qt::NoBrush);
                painter.drawEllipse(center, imgW / 2 + 5, imgH / 2 + 5);
            }
            
            // scaled = addDropShadowEffect(scaled);

            painter.drawPixmap(center.x() - imgW / 2,
                               center.y() - imgH / 2,
                               scaled);
        }
    }
}


void ChessBoardWidget::loadPieceImages()
{
    piecePixmaps.clear();
    QStringList colors = { "r", "b" };
    QStringList types  = { "a", "b", "c", "k", "n", "p", "r" };

    if (!customPieceDir.isEmpty())
    {
        QDir dir(customPieceDir);

        // 1. XỬ LÝ SPRITESHEET (Dành cho các bộ như Plastic có chessred.png / chessblack.png)
        QPixmap redSprite(dir.filePath("chessred.png"));
        QPixmap blackSprite(dir.filePath("chessblack.png"));

        if (!redSprite.isNull() && !blackSprite.isNull())
        {
            qDebug() << "[ChessBoardWidget] Phát hiện Spritesheet, đang cắt ảnh...";
            int pieceW = redSprite.width() / 7;
            int pieceH = redSprite.height();

            // Thứ tự quân cờ phổ biến trên Spritesheet: Xe, Mã, Tượng, Sĩ, Tướng, Pháo, Tốt
            QStringList spriteOrder = {"r", "n", "b", "a", "k", "c", "p"};

            for (int i = 0; i < 7; ++i) {
                piecePixmaps["r" + spriteOrder[i]] = redSprite.copy(i * pieceW, 0, pieceW, pieceH);
                piecePixmaps["b" + spriteOrder[i]] = blackSprite.copy(i * pieceW, 0, pieceW, pieceH);
            }
            return; // Đã cắt xong từ Spritesheet thì thoát luôn
        }

        // 2. XỬ LÝ ẢNH FILE LẺ (Có thể nằm trong thư mục con)
        QString sizeFolder;
        switch (pieceSize) {
        case PieceSize::Small:  sizeFolder = "small"; break;
        case PieceSize::Medium: sizeFolder = "middle"; break;
        case PieceSize::Large:  sizeFolder = "large"; break;
        default: sizeFolder = "large"; break;
        }

        if (dir.exists(sizeFolder)) {
            dir.cd(sizeFolder);
        }

        for (const auto& pieceType : types) {
            for (const auto& color : colors) {
                QString baseNameLower = QString(color) + QString(pieceType);
                QString baseNameUpper = baseNameLower.toUpper();

                QStringList possibleFiles = {
                    baseNameLower + ".png", baseNameLower + ".bmp",
                    baseNameUpper + ".png", baseNameUpper + ".bmp"
                };

                QString fullPath;
                for (const QString& fileName : possibleFiles) {
                    if (dir.exists(fileName)) {
                        fullPath = dir.filePath(fileName);
                        break;
                    }
                }

                if (!fullPath.isEmpty()) {
                    piecePixmaps[baseNameLower] = QPixmap(fullPath);
                }
            }
        }
    }
    else
    {
        // 3. LOAD QUÂN CỜ MẶC ĐỊNH TỪ HỆ THỐNG
        QString folder;
        switch (pieceSize)
        {
        case PieceSize::Small:  folder =  baseImagePath + "/small/";  break;
        case PieceSize::Medium: folder =  baseImagePath + "/middle/"; break;
        case PieceSize::Large:  folder =  baseImagePath + "/large/";  break;
        default:                folder =  baseImagePath + "/small/";  break;
        }

        for (const auto& pieceType : types) {
            for (const auto& color : colors) {
                QString fileName = QString(color) + QString(pieceType) + ".bmp";
                QString fullPath = folder + fileName;
                QPixmap pixmap(fullPath);
                if (!pixmap.isNull()) {
                    piecePixmaps[QString(color) + QString(pieceType)] = pixmap;
                }
            }
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

    QString colorPrefix = piece.isRed ? "r" : "b";
    QString typeStr;

    switch (piece.type)
    {
        case PieceType::Advisor: typeStr = "a"; break;
        case PieceType::Bishop:  typeStr = "b"; break;
        case PieceType::Cannon:  typeStr = "c"; break;
        case PieceType::King:    typeStr = "k"; break;
        case PieceType::Knight:  typeStr = "n"; break;
        case PieceType::Pawn:    typeStr = "p"; break;
        case PieceType::Rook:    typeStr = "r"; break;
        case PieceType::None:
        default:
            return QPixmap();
    }

    QString key = colorPrefix + typeStr;
    return piecePixmaps.value(key);
}


// ***************************************************************************************

void ChessBoardWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint pos = event->position().toPoint();

    int cellW = calculatedCellWidth;
    int cellH = calculatedCellHeight;
    int leftMargin = calculatedLeftMargin;
    int topMargin = calculatedTopMargin;

    if (cellW == 0 || cellH == 0)
    {
        // Should not happen since calculateBoardMetrics() always sets values,
        // but provide a safe fallback just in case
        cellW = 40;
        cellH = 40;
        leftMargin = 56;
        topMargin = 56;
    }

    // Calculate which cell was clicked based on the margins and cell size
    int col = (pos.x() - leftMargin + cellW / 2) / cellW;
    int row = (pos.y() - topMargin + cellH / 2) / cellH;

    if (row >= 0 && row < 10 && col >= 0 && col < 9)
    {
        QPoint clickedCell(col, row);
        const Piece& clickedPiece = board[row][col];
        
        if (selectedCell == QPoint(-1, -1)) {
            // No piece selected yet - select this piece if it exists
            if (!clickedPiece.isEmpty()) {
                // Check turn: can only select pieces of the current player
                if (clickedPiece.isRed != isRedTurn) {
                    // Invalid selection - wrong turn
                    QString pieceName = clickedPiece.toString();
                    QString moveDesc = QString("from (%1,%2) to (%3,%4)").arg(row).arg(col).arg(row).arg(col);
                    emit invalidMoveAttempted(pieceName, moveDesc);
                    qDebug() << "[ChessBoardWidget] Invalid selection attempted (wrong turn):" << pieceName;
                    return;
                }
                selectedCell = clickedCell;
                // Calculate valid moves for this piece
                validMoves = getValidMovesForPiece(row, col);
                qDebug() << "[ChessBoardWidget] Selected piece at (" << row << "," << col << ") with" << validMoves.size() << "valid moves";
            }
        } else {
            // A piece is already selected - try to move it
            if (selectedCell == clickedCell) {
                // Clicked same cell - deselect
                selectedCell = QPoint(-1, -1);
                validMoves.clear();
            } else if (validMoves.contains(clickedCell)) {
                // Clicked on a valid move destination - execute move
                int fromRow = selectedCell.y();
                int fromCol = selectedCell.x();
                int toRow = clickedCell.y();
                int toCol = clickedCell.x();
                
                const Piece& movingPiece = board[fromRow][fromCol];
                
                // Check turn: can only move pieces of the current player
                if (movingPiece.isRed != isRedTurn) {
                    // Invalid move - wrong turn
                    QString pieceName = movingPiece.toString();
                    QString moveDesc = QString("from (%1,%2) to (%3,%4)").arg(fromRow).arg(fromCol).arg(toRow).arg(toCol);
                    emit invalidMoveAttempted(pieceName, moveDesc);
                    qDebug() << "[ChessBoardWidget] Invalid move attempted (wrong turn):" << pieceName;
                    return;
                }
                
                if (isValidMove(movingPiece, fromRow, fromCol, toRow, toCol)) {
                    // 1. Ghi nhận tọa độ nước đi
                    currentMoveStart = QPoint(fromCol, fromRow);
                    currentMoveEnd = QPoint(toCol, toRow);

                    // 2. Lưu lịch sử TRƯỚC KHI cập nhật board (để lấy đúng quân cờ)
                    saveMoveToHistory();

                    // 3. Thực thi nước đi trên mảng board
                    board[toRow][toCol] = board[fromRow][fromCol];
                    board[fromRow][fromCol] = Piece::empty();

                    // 4. Đổi lượt
                    isRedTurn = !isRedTurn;

                    qDebug() << "[ChessBoardWidget] Move executed:" << fromRow << "," << fromCol << "->" << toRow << "," << toCol;
                } else {
                    // Invalid move - show error
                    QString pieceName = movingPiece.toString();
                    QString moveDesc = QString("from (%1,%2) to (%3,%4)").arg(fromRow).arg(fromCol).arg(toRow).arg(toCol);
                    emit invalidMoveAttempted(pieceName, moveDesc);
                    qDebug() << "[ChessBoardWidget] Invalid move attempted:" << moveDesc;
                }
                
                // Clear selection after move attempt
                selectedCell = QPoint(-1, -1);
                validMoves.clear();
            } else {
                // Clicked on another piece - select it instead if it's the same color
                if (!clickedPiece.isEmpty() && clickedPiece.isRed == board[selectedCell.y()][selectedCell.x()].isRed) {
                    // Check turn: can only select pieces of the current player
                    if (clickedPiece.isRed != isRedTurn) {
                        // Invalid selection - wrong turn
                        QString pieceName = clickedPiece.toString();
                        QString moveDesc = QString("from (%1,%2) to (%3,%4)").arg(row).arg(col).arg(row).arg(col);
                        emit invalidMoveAttempted(pieceName, moveDesc);
                        qDebug() << "[ChessBoardWidget] Invalid selection attempted (wrong turn):" << pieceName;
                        return;
                    }
                    selectedCell = clickedCell;
                    validMoves = getValidMovesForPiece(row, col);
                    qDebug() << "[ChessBoardWidget] Switched selection to piece at (" << row << "," << col << ")";
                } else {
                    // Clicked on opponent piece or empty square that's not a valid move - deselect
                    selectedCell = QPoint(-1, -1);
                    validMoves.clear();
                }
            }
        }
        
        update();
    }
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


QPoint ChessBoardWidget::cellToPixel(int row, int col) const
{
    int cellW = calculatedCellWidth;
    int cellH = calculatedCellHeight;
    int leftMargin = calculatedLeftMargin;
    int topMargin = calculatedTopMargin;

    if (cellW == 0 || cellH == 0)
    {
        // Should not happen since calculateBoardMetrics() always sets values,
        // but provide a safe fallback just in case
        cellW = 40;
        cellH = 40;
        leftMargin = 56;
        topMargin = 56;
    }

    int centerX = leftMargin + col * cellW;
    int centerY = topMargin + row * cellH;

    return QPoint(centerX, centerY);
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
    // 1. Cập nhật kích thước bàn cờ
    boardSize = size;

    // 2. Đồng bộ kích thước quân cờ tương ứng
    switch (size) {
    case BoardSize::Small:  pieceSize = PieceSize::Small;  break;
    case BoardSize::Medium: pieceSize = PieceSize::Medium; break;
    case BoardSize::Large:  pieceSize = PieceSize::Large;  break;
    default:                pieceSize = PieceSize::Medium; break;
    }

    // 3. Tải ảnh bàn cờ mới và cập nhật kích thước Widget
    loadBoardImage(size);
    updateSizeFromBoard();

    // 4. QUAN TRỌNG NHẤT: Tính toán lại kích thước lưới và quân cờ
    if (customBoardPath.isEmpty()) {
        calculateBoardMetrics(); // Lấy thông số chuẩn (hardcode) nếu là bàn cờ hệ thống
    } else {
        calculateBoardMetricsFromImage(boardImage); // Tính tự động nếu đang dùng bàn cờ custom
    }

    // 5. Tải lại bộ nhớ đệm ảnh quân cờ (để lấy đúng thư mục small/middle/large)
    loadPieceImages();

    // 6. Kích hoạt vẽ lại giao diện
    update();
}


void ChessBoardWidget::loadCustomBoard(const QString& path)
{
    customBoardPath = path;
    setBoardSize(currentSize); // Reloads the board and resizes
}

void ChessBoardWidget::loadCustomPieces(const QString& dirPath)
{
    customPieceDir = dirPath;
    loadPieceImages(); // Reloads pieces into cache
    update();
}

void ChessBoardWidget::resetCustomTheme()
{
    customBoardPath.clear();
    customPieceDir.clear();
    setBoardSize(currentSize);
    loadPieceImages();
    update();
}

QSize ChessBoardWidget::sizeHint() const
{
    if (boardImage.isNull())
        return QSize(400, 400); // fallback

    QSize sz = boardImage.size(); // or calculated by boardSize
    qDebug() << "[sizeHint] returning: " << sz;
    return sz;
}

void ChessBoardWidget::analyzeBoardGrid()
{
    // Intentionally empty — grid detection via pixel scanning is unreliable
    // for SharkChess-style golden boards. Use calculateBoardMetrics() instead,
    // which provides precise hardcoded measurements for each board size.
}

void ChessBoardWidget::calculateBoardMetrics()
{
    // Precise grid measurements for each board size.
    // These values were obtained by analyzing the actual board images:
    //   Large  (572x626): first grid intersection at (71, 68),  cell 53.75 x 54.11
    //   Medium (432x473): first grid intersection at (56, 56),  cell 40 x 40
    //   Small  (292x318): first grid intersection at (42, 41),  cell 26 x 26
    //
    // The Chinese chess board has 9 columns (8 intervals) and 10 rows (9 intervals).

    switch (boardSize)
    {
    case BoardSize::Large:
        calculatedLeftMargin = 71;
        calculatedTopMargin  = 68;
        calculatedCellWidth  = 54;  // (501 - 71) / 8 ≈ 53.75, rounded to 54
        calculatedCellHeight = 54;  // (555 - 68) / 9 ≈ 54.11, rounded to 54
        break;

    case BoardSize::Medium:
        calculatedLeftMargin = 56;
        calculatedTopMargin  = 56;
        calculatedCellWidth  = 40;  // (376 - 56) / 8 = 40
        calculatedCellHeight = 40;  // (416 - 56) / 9 = 40
        break;

    case BoardSize::Small:
        calculatedLeftMargin = 42;
        calculatedTopMargin  = 41;
        calculatedCellWidth  = 26;  // (250 - 42) / 8 = 26
        calculatedCellHeight = 26;  // (275 - 41) / 9 = 26
        break;

    case BoardSize::Mini:
        calculatedLeftMargin = 10;
        calculatedTopMargin  = 12;
        calculatedCellWidth  = 16;
        calculatedCellHeight = 16;
        break;
    }

    // Piece size should be slightly smaller than cell size to avoid overlap
    calculatedPieceSize = static_cast<int>(qMin(calculatedCellWidth, calculatedCellHeight) * 0.90);

    qDebug() << "Board metrics set for size" << static_cast<int>(boardSize) << ":"
             << "margins(" << calculatedLeftMargin << "," << calculatedTopMargin << ")"
             << "cell(" << calculatedCellWidth << "x" << calculatedCellHeight << ")"
             << "pieceSize" << calculatedPieceSize;
}


// ----------------------------------------------------

void ChessBoardWidget::showContextMenu(const QPoint &pos)
{
    QMenu menu(this);

    menu.addMenu(pieceSizeMenu);
    MenuUtils::addFullWidthSeparator(&menu, 2);
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
    MenuUtils::addFullWidthSeparator(&menu, 2);

    menu.addMenu(customThemeMenu);
    MenuUtils::addFullWidthSeparator(&menu, 2);
    menu.addMenu(m_pieceSetMenu);
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

void ChessBoardWidget::onLoadCustomBoardRequested() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Board Image"));
    if (!fileName.isEmpty()) {
        loadCustomBoard(fileName); // Hàm của bạn đã có sẵn
    }
}

void ChessBoardWidget::onLoadCustomPiecesRequested() {
    QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Pieces Folder"));
    if (!folderPath.isEmpty()) {
        loadCustomPieces(folderPath); // Hàm của bạn đã có sẵn
    }
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
    // move.movedPiece = board[currentMoveStart.x()][currentMoveStart.y()];
    // move.capturedPiece = board[currentMoveEnd.x()][currentMoveEnd.y()];

    move.movedPiece = board[currentMoveStart.y()][currentMoveStart.x()];
    move.capturedPiece = board[currentMoveEnd.y()][currentMoveEnd.x()];

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

// Board and piece selection methods
void ChessBoardWidget::onBoardChanged(const QString& category, const QString& imagePath)
{
    qDebug() << "[ChessBoardWidget] Board changed: category=" << category << "imagePath=" << imagePath;
    
    if (imagePath.isEmpty()) {
        // Reset to default board
        loadBoardImage(currentSize);
    } else {
        // Load custom board image
        QPixmap newBoard(imagePath);
        if (!newBoard.isNull()) {
            boardImage = newBoard;
            updateSizeFromBoard();
            calculateBoardMetricsFromImage(boardImage);
            update();
        }
    }
}

void ChessBoardWidget::loadPieceSet(const QString& pieceSetPath)
{
    qDebug() << "[ChessBoardWidget] Loading piece set from:" << pieceSetPath;
    customPieceDir = pieceSetPath;
    PieceLoader::setPieceSetPrefix(pieceSetPath);

    loadPieceImages();
    update();
}

void ChessBoardWidget::calculateBoardMetricsFromImage(const QPixmap& boardImage)
{
    // Calculate board metrics based on the actual board image dimensions
    // This is a simplified version - in practice, you might want to detect grid lines
    int boardWidth = boardImage.width();
    int boardHeight = boardImage.height();
    
    // Chinese chess board has 9 columns (8 intervals) and 10 rows (9 intervals)
    // Estimate margins and cell sizes based on standard proportions
    // These are approximate and may need adjustment based on actual board images
    
    // Typical margins are around 10-15% of board dimensions
    calculatedLeftMargin = static_cast<int>(boardWidth * 0.12);
    calculatedTopMargin = static_cast<int>(boardHeight * 0.11);
    
    // Calculate cell dimensions
    int availableWidth = boardWidth - 2 * calculatedLeftMargin;
    int availableHeight = boardHeight - 2 * calculatedTopMargin;
    
    calculatedCellWidth = availableWidth / 8;  // 8 intervals for 9 columns
    calculatedCellHeight = availableHeight / 9; // 9 intervals for 10 rows
    
    // Piece size should be slightly smaller than cell size
    calculatedPieceSize = static_cast<int>(qMin(calculatedCellWidth, calculatedCellHeight) * 0.85);
    
    qDebug() << "[ChessBoardWidget] Calculated metrics for custom board:"
             << "margins(" << calculatedLeftMargin << "," << calculatedTopMargin << ")"
             << "cell(" << calculatedCellWidth << "x" << calculatedCellHeight << ")"
             << "pieceSize" << calculatedPieceSize;
}

// Move validation for Chinese Chess pieces
bool ChessBoardWidget::isValidMove(const Piece& piece, int fromRow, int fromCol, int toRow, int toCol) const
{
    // Check bounds
    if (fromRow < 0 || fromRow >= 10 || fromCol < 0 || fromCol >= 9 ||
        toRow < 0 || toRow >= 10 || toCol < 0 || toCol >= 9) {
        return false;
    }
    
    // Can't move to same position
    if (fromRow == toRow && fromCol == toCol) {
        return false;
    }
    
    // Check if destination has own piece
    const Piece& targetPiece = board[toRow][toCol];
    if (!targetPiece.isEmpty() && targetPiece.isRed == piece.isRed) {
        return false;
    }
    
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;
    int absRowDiff = qAbs(rowDiff);
    int absColDiff = qAbs(colDiff);
    
    bool isRed = piece.isRed;
    
    switch (piece.type) {
        case PieceType::King: {
            // King moves 1 step orthogonally within palace (3x3)
            // Red palace: rows 7-9, cols 3-5
            // Black palace: rows 0-2, cols 3-5
            if (absRowDiff + absColDiff != 1) return false; // Must move 1 step orthogonally
            
            // Check palace boundaries
            if (isRed) {
                return toRow >= 7 && toRow <= 9 && toCol >= 3 && toCol <= 5;
            } else {
                return toRow >= 0 && toRow <= 2 && toCol >= 3 && toCol <= 5;
            }
        }
        
        case PieceType::Advisor: {
            // Advisor moves 1 step diagonally within palace
            if (absRowDiff != 1 || absColDiff != 1) return false;
            
            // Check palace boundaries
            if (isRed) {
                return toRow >= 7 && toRow <= 9 && toCol >= 3 && toCol <= 5;
            } else {
                return toRow >= 0 && toRow <= 2 && toCol >= 3 && toCol <= 5;
            }
        }
        
        case PieceType::Bishop: {
            // Bishop moves 2 steps diagonally, cannot cross river
            if (absRowDiff != 2 || absColDiff != 2) return false;
            
            // Check for blocking piece at the "eye" (midpoint)
            int eyeRow = fromRow + rowDiff / 2;
            int eyeCol = fromCol + colDiff / 2;
            if (!board[eyeRow][eyeCol].isEmpty()) return false;
            
            // Cannot cross river
            if (isRed) {
                return toRow >= 5; // Red side (rows 5-9)
            } else {
                return toRow <= 4; // Black side (rows 0-4)
            }
        }
        
        case PieceType::Knight: {
            // Knight moves in L-shape: 2 orthogonal + 1 diagonal
            // Check for blocking piece at the "leg"
            if (absRowDiff == 2 && absColDiff == 1) {
                // Vertical move 2, horizontal 1
                int legRow = fromRow + rowDiff / 2;
                int legCol = fromCol;
                if (!board[legRow][legCol].isEmpty()) return false;
                return true;
            } else if (absRowDiff == 1 && absColDiff == 2) {
                // Horizontal move 2, vertical 1
                int legRow = fromRow;
                int legCol = fromCol + colDiff / 2;
                if (!board[legRow][legCol].isEmpty()) return false;
                return true;
            }
            return false;
        }
        
        case PieceType::Rook: {
            // Rook moves orthogonally any distance, no jumping
            if (rowDiff != 0 && colDiff != 0) return false; // Must move in straight line
            
            // Check path is clear
            if (rowDiff != 0) {
                int step = rowDiff > 0 ? 1 : -1;
                for (int r = fromRow + step; r != toRow; r += step) {
                    if (!board[r][fromCol].isEmpty()) return false;
                }
            } else {
                int step = colDiff > 0 ? 1 : -1;
                for (int c = fromCol + step; c != toCol; c += step) {
                    if (!board[fromRow][c].isEmpty()) return false;
                }
            }
            return true;
        }
        
        case PieceType::Cannon: {
            // Cannon moves like rook but captures by jumping over exactly one piece
            if (rowDiff != 0 && colDiff != 0) return false; // Must move in straight line
            
            int piecesBetween = 0;
            if (rowDiff != 0) {
                int step = rowDiff > 0 ? 1 : -1;
                for (int r = fromRow + step; r != toRow; r += step) {
                    if (!board[r][fromCol].isEmpty()) piecesBetween++;
                }
            } else {
                int step = colDiff > 0 ? 1 : -1;
                for (int c = fromCol + step; c != toCol; c += step) {
                    if (!board[fromRow][c].isEmpty()) piecesBetween++;
                }
            }
            
            // If capturing, must have exactly one piece between
            // If not capturing, must have zero pieces between
            if (!targetPiece.isEmpty()) {
                return piecesBetween == 1;
            } else {
                return piecesBetween == 0;
            }
        }
        
        case PieceType::Pawn: {
            // Pawn moves forward 1 step, after crossing river can move left/right 1 step
            // Red pawns move up (decreasing row), Black pawns move down (increasing row)
            if (isRed) {
                // Red pawn moves up (row decreases)
                if (colDiff == 0 && rowDiff == -1) return true; // Forward
                if (fromRow <= 4 && absColDiff == 1 && rowDiff == 0) return true; // Left/right after river
                return false;
            } else {
                // Black pawn moves down (row increases)
                if (colDiff == 0 && rowDiff == 1) return true; // Forward
                if (fromRow >= 5 && absColDiff == 1 && rowDiff == 0) return true; // Left/right after river
                return false;
            }
        }
        
        default:
            return false;
    }
}

QVector<QPoint> ChessBoardWidget::getValidMovesForPiece(int row, int col) const
{
    QVector<QPoint> validMoves;
    
    if (row < 0 || row >= 10 || col < 0 || col >= 9)
        return validMoves;
    
    const Piece& piece = board[row][col];
    if (piece.isEmpty()) {
        return validMoves;
    }
    
    // Check all possible destination squares
    for (int toRow = 0; toRow < 10; ++toRow) {
        for (int toCol = 0; toCol < 9; ++toCol) {
            if (isValidMove(piece, row, col, toRow, toCol)) {
                validMoves.append(QPoint(toCol, toRow));
            }
        }
    }
    
    return validMoves;
}

void ChessBoardWidget::onLoadCustomBoard()
{
    QString dir = QCoreApplication::applicationDirPath();
    QString fileName = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select Custom Board Image"),
        dir,
        tr("Images (*.png *.jpg *.jpeg *.bmp);;All Files (*)")
        );

    if (!fileName.isEmpty())
        loadCustomBoard(fileName);
}

void ChessBoardWidget::onLoadCustomPieces()
{
    QString dir = QCoreApplication::applicationDirPath();
    QString folderPath = QFileDialog::getExistingDirectory(
        nullptr,
        tr("Select Custom Pieces Directory"),
        dir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!folderPath.isEmpty())
        loadCustomPieces(folderPath);
}

void ChessBoardWidget::onResetCustomTheme()
{
    resetCustomTheme();
}

void ChessBoardWidget::onLoadPieceSet()
{
    QString dir = QCoreApplication::applicationDirPath() + "/Background cho quan co";
    QString folderPath = QFileDialog::getExistingDirectory(
        nullptr,
        tr("Select Piece Set Directory"),
        dir,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!folderPath.isEmpty())
       loadPieceSet(folderPath);
}

void ChessBoardWidget::onLoadPieceSet(const QString& pieceSetPath)
{
    loadPieceSet(pieceSetPath);
}

void ChessBoardWidget::onLoadPieceSetFromCategory()
{
    const QMap<QString, QString>& categories = getBoardCategories();
    QStringList items;
    for (auto it = categories.begin(); it != categories.end(); ++it) {
        if (it.key().startsWith("Pieces/")) {
            items << it.key().mid(7); // Chỉ lấy phần tên hiển thị
        }
    }

    bool ok;
    QString item = QInputDialog::getItem(nullptr, tr("Select Piece Set"),
                                         tr("Choose a category:"), items, 0, false, &ok);

    if (ok && !item.isEmpty()) {
        // Tìm lại path dựa trên tên đã chọn
        QString fullCategoryName = "Pieces/" + item;
        if (categories.contains(fullCategoryName)) {
            onLoadPieceSet(categories.value(fullCategoryName));
        }
    }
}


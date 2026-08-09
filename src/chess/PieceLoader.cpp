#include "PieceLoader.h"
#include <QDebug>
#include <QPainter>

static QString g_pieceSetPrefix = ":/img";

void PieceLoader::setPieceSetPrefix(const QString& prefix)
{
    g_pieceSetPrefix = prefix;
}

QString PieceLoader::getPieceSetPrefix()
{
    return g_pieceSetPrefix;
}

QPixmap PieceLoader::loadPiecePixmap(PieceType type, bool isRed, ChessBoardWidget::BoardSize boardSize, const QString& baseThemePath)
{
    QString baseFolder = g_pieceSetPrefix.isEmpty() ? ":/img" : g_pieceSetPrefix;
    if (baseFolder == "img" || baseFolder.startsWith("img/")) {
        baseFolder = ":/" + baseFolder;
    }

    QString sizeFolder;

    switch (boardSize)
    {
        case ChessBoardWidget::BoardSize::Small:  sizeFolder = baseFolder + "/small"; break;
        case ChessBoardWidget::BoardSize::Medium: sizeFolder = baseFolder + "/middle"; break;
        case ChessBoardWidget::BoardSize::Large:  sizeFolder = baseFolder + "/large"; break;
        case ChessBoardWidget::BoardSize::Mini:   sizeFolder = baseFolder + "/mini"; break;

        default:
            qWarning() << "Unknown board size!";
            return QPixmap();
    }

    QString colorPrefix = isRed ? "r" : "b";
    QString typeStr;

    switch (type)
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
            return QPixmap(); // no peace that doesn't load
    }

    QString fileName;
    if (boardSize == ChessBoardWidget::BoardSize::Mini) {
        fileName = QString("%1%2.bmp").arg(colorPrefix.toUpper(), typeStr.toUpper());
    } else {
        fileName = QString("%1%2.bmp").arg(colorPrefix, typeStr);
    }

    QString path = sizeFolder + "/" + fileName;
    QPixmap pix(path);
    if (pix.isNull())
    {
        QString customPath = baseFolder + "/" + fileName;
        pix.load(customPath);

        if (pix.isNull()) {
            customPath.replace(".bmp", ".png");
            pix.load(customPath);
        }

        if (pix.isNull())
            qWarning() << "Not find image:" << path << " or " << customPath << "\n";
    }
    return pix;
}

void PieceLoader::drawPieceAt(QPainter& painter, PieceType type, bool isRed, int row, int col,
                              int cellSize, ChessBoardWidget::BoardSize size, const ChessBoardWidget* boardWidget)
{
    qDebug() << "Draw piece at row:" << row << "col:" << col << "with board size:" << static_cast<int>(size);

    QPixmap pix = loadPiecePixmap(type, isRed, size);
    if (pix.isNull())
    {
        qWarning() << "Pixmap is null!";
        return;
    }

    // Calculate the proportions for the pieces based on the cell size
    QPixmap scaledPixmap = pix.scaled(cellSize, cellSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPoint center = boardWidget->cellToPixel(row, col);
    int imgW = scaledPixmap.width();
    int imgH = scaledPixmap.height();

    // draw peace on chessboard
    painter.drawPixmap(center.x() - imgW / 2, center.y() - imgH / 2, scaledPixmap);
}



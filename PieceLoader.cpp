#include "PieceLoader.h"
#include <QDebug>
#include <QPainter>

QPixmap PieceLoader::loadPiecePixmap(PieceType type, bool isRed, ChessBoardWidget::BoardSize boardSize)
{
    QString sizeFolder;

    switch (boardSize)
    {
        case ChessBoardWidget::BoardSize::Small:  sizeFolder = "img/small"; break;
        case ChessBoardWidget::BoardSize::Medium: sizeFolder = "img/middle"; break;
        case ChessBoardWidget::BoardSize::Large:  sizeFolder = "img/large"; break;
        case ChessBoardWidget::BoardSize::Mini:   sizeFolder = "img/mini"; break;

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

    QString path;
    if (boardSize == ChessBoardWidget::BoardSize::Mini)
    {
        // for mini, file name must uppercase: ex "RA.bmp"
        path = QString("%1/%2%3.bmp").arg(sizeFolder, colorPrefix.toUpper(), typeStr.toUpper());
    }
    else
    {
        // others size still use lowercase: ex "ra.bmp"
        path = QString("%1/%2%3.bmp").arg(sizeFolder, colorPrefix, typeStr);
    }

    QPixmap pix(path);
    if (pix.isNull())
        qWarning() << "Not find image:" << path;

    return pix;
}

void PieceLoader::drawPieceAt(QPainter& painter, PieceType type, bool isRed, int row, int col,
                              int cellSize, ChessBoardWidget::BoardSize size)
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

    const ChessBoardWidget* boardWidget;
    // Calculate the position of the chess piece in the cell
    QPoint center = boardWidget->cellToPixel(row, col);
    int imgW = scaledPixmap.width();
    int imgH = scaledPixmap.height();

    // draw peace on chessboard
    painter.drawPixmap(center.x() - imgW / 2, center.y() - imgH / 2, scaledPixmap);
}



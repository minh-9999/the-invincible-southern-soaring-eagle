#ifndef PIECELOADER_H
#define PIECELOADER_H

#include "Piece.h"     // store enum class PieceType
#include "ChessBoardWidget.h"

namespace PieceLoader
{
QPixmap loadPiecePixmap(PieceType type, bool isRed, ChessBoardWidget::BoardSize boardSize);

void drawPieceAt(QPainter& painter, PieceType type, bool isRed, int row, int col,
                 int cellSize, ChessBoardWidget::BoardSize size);


}

#endif // PIECELOADER_H

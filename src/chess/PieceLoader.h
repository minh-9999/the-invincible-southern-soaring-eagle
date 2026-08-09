#ifndef PIECELOADER_H
#define PIECELOADER_H

#include "Piece.h"     // store enum class PieceType
#include "ChessBoardWidget.h"

namespace PieceLoader
{

void setPieceSetPrefix(const QString& prefix);

QString getPieceSetPrefix();

QPixmap loadPiecePixmap(PieceType type, bool isRed, ChessBoardWidget::BoardSize boardSize, const QString& baseThemePath = "");

// void drawPieceAt(QPainter& painter, PieceType type, bool isRed, int row, int col,
//                  int cellSize, ChessBoardWidget::BoardSize size);

void drawPieceAt(QPainter& painter, PieceType type, bool isRed, int row, int col,
                 int cellSize, ChessBoardWidget::BoardSize size, const ChessBoardWidget* boardWidget);


}

#endif // PIECELOADER_H

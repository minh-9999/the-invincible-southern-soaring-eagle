#ifndef MOVE_DATA_H
#define MOVE_DATA_H

#include "Piece.h"
#include <QString>
#include <QPoint>

struct Move
{
    int fromX, fromY;
    int toX, toY;
    QString notation; // Ex: "炮二平五"

    QPoint start;
    QPoint end;
    Piece movedPiece;
    Piece capturedPiece;

};

#endif // MOVE_DATA_H

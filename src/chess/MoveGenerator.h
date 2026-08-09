#ifndef MOVEGENERATOR_H
#define MOVEGENERATOR_H

#include "Piece.h"
#include <QVector>
#include <QPoint>

/**
 * @brief Xiangqi (Chinese Chess) legal move generator.
 *
 * Generates strictly legal moves for any piece on the board by first computing
 * pseudo-legal moves, then filtering out those that violate the "Flying General"
 * rule (two generals facing each other on the same file with no pieces between).
 */
class MoveGenerator
{
public:
    /// Returns a list of legal destination coordinates (col, row) for the piece at (row, col).
    static QVector<QPoint> generateValidMoves(const QVector<QVector<Piece>>& board, int row, int col);

private:
    static void getKingMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);
    static void getAdvisorMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);
    static void getBishopMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);
    static void getKnightMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);
    static void getRookMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);
    static void getCannonMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);
    static void getPawnMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves);

    // Helpers
    static bool isValidPos(int r, int c);
    static bool isSameSide(const Piece& p1, const Piece& p2);
    
    /// Check if the two generals are facing each other ("Flying General" rule violation).
    static bool isKingsFacing(const QVector<QVector<Piece>>& board);
};

#endif // MOVEGENERATOR_H

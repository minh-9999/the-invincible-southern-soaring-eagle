#include "MoveGenerator.h"

bool MoveGenerator::isValidPos(int r, int c)
{
    return r >= 0 && r < 10 && c >= 0 && c < 9;
}

bool MoveGenerator::isSameSide(const Piece& p1, const Piece& p2)
{
    if (p1.isEmpty() || p2.isEmpty()) return false;
    return p1.isRed == p2.isRed;
}

bool MoveGenerator::isKingsFacing(const QVector<QVector<Piece>>& board)
{
    int redKingCol = -1, redKingRow = -1;
    int blackKingCol = -1, blackKingRow = -1;

    // Find the kings
    for (int r = 0; r < 10; ++r) {
        for (int c = 3; c <= 5; ++c) {
            const Piece& p = board[r][c];
            if (p.type == PieceType::King) {
                if (p.isRed) {
                    redKingRow = r;
                    redKingCol = c;
                } else {
                    blackKingRow = r;
                    blackKingCol = c;
                }
            }
        }
    }

    if (redKingCol == -1 || blackKingCol == -1) return false;
    
    // Kings must be in the same column to face each other
    if (redKingCol != blackKingCol) return false;
    
    // Check if there's any piece between them
    int startRow = qMin(redKingRow, blackKingRow) + 1;
    int endRow = qMax(redKingRow, blackKingRow) - 1;
    
    for (int r = startRow; r <= endRow; ++r) {
        if (!board[r][redKingCol].isEmpty()) {
            return false; // A piece is blocking the view
        }
    }
    
    return true; // Kings are facing directly
}

QVector<QPoint> MoveGenerator::generateValidMoves(const QVector<QVector<Piece>>& board, int row, int col)
{
    QVector<QPoint> pseudoLegalMoves;
    
    if (!isValidPos(row, col)) return pseudoLegalMoves;
    
    const Piece& piece = board[row][col];
    if (piece.isEmpty()) return pseudoLegalMoves;

    switch (piece.type) {
        case PieceType::King:
            getKingMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        case PieceType::Advisor:
            getAdvisorMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        case PieceType::Bishop:
            getBishopMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        case PieceType::Knight:
            getKnightMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        case PieceType::Rook:
            getRookMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        case PieceType::Cannon:
            getCannonMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        case PieceType::Pawn:
            getPawnMoves(board, row, col, piece.isRed, pseudoLegalMoves);
            break;
        default:
            break;
    }

    // Filter out pseudo-legal moves that violate the "Flying General" rule
    QVector<QPoint> legalMoves;
    for (const QPoint& move : pseudoLegalMoves) {
        // Create a temporary board to simulate the move
        QVector<QVector<Piece>> tempBoard = board;
        tempBoard[move.y()][move.x()] = tempBoard[row][col];
        tempBoard[row][col] = Piece::empty();
        
        // If the generals do not face each other after the move, it is legal
        if (!isKingsFacing(tempBoard)) {
            legalMoves.append(move);
        }
    }

    return legalMoves;
}

void MoveGenerator::getKingMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    // Palace boundaries:
    // Black (top): rows 0-2, columns 3-5
    // Red (bottom): rows 7-9, columns 3-5
    int minRow = isRed ? 7 : 0;
    int maxRow = isRed ? 9 : 2;
    int minCol = 3, maxCol = 5;

    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        
        if (nr >= minRow && nr <= maxRow && nc >= minCol && nc <= maxCol) {
            if (!isSameSide(board[row][col], board[nr][nc])) {
                moves.append(QPoint(nc, nr)); // QPoint uses (x, y) = (col, row)
            }
        }
    }
}

void MoveGenerator::getAdvisorMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    int dr[] = {-1, -1, 1, 1};
    int dc[] = {-1, 1, -1, 1};
    
    int minRow = isRed ? 7 : 0;
    int maxRow = isRed ? 9 : 2;
    int minCol = 3, maxCol = 5;

    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        
        if (nr >= minRow && nr <= maxRow && nc >= minCol && nc <= maxCol) {
            if (!isSameSide(board[row][col], board[nr][nc])) {
                moves.append(QPoint(nc, nr));
            }
        }
    }
}

void MoveGenerator::getBishopMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    int dr[] = {-2, -2, 2, 2};
    int dc[] = {-2, 2, -2, 2};
    
    // Elephant eye positions (blocking point for each diagonal)
    int eye_dr[] = {-1, -1, 1, 1};
    int eye_dc[] = {-1, 1, -1, 1};
    
    int minRow = isRed ? 5 : 0; // Elephants cannot cross the river
    int maxRow = isRed ? 9 : 4;

    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        int eye_r = row + eye_dr[i];
        int eye_c = col + eye_dc[i];
        
        if (nr >= minRow && nr <= maxRow && nc >= 0 && nc < 9) {
            // Check if the elephant eye is blocked
            if (board[eye_r][eye_c].isEmpty()) {
                if (!isSameSide(board[row][col], board[nr][nc])) {
                    moves.append(QPoint(nc, nr));
                }
            }
        }
    }
}

void MoveGenerator::getKnightMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};
    
    // Horse leg blocking positions
    int block_dr[] = {-1, -1, 0, 0, 0, 0, 1, 1};
    int block_dc[] = {0, 0, -1, 1, -1, 1, 0, 0};

    for (int i = 0; i < 8; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        int block_r = row + block_dr[i];
        int block_c = col + block_dc[i];
        
        if (isValidPos(nr, nc)) {
            // Check if the horse's leg is blocked
            if (board[block_r][block_c].isEmpty()) {
                if (!isSameSide(board[row][col], board[nr][nc])) {
                    moves.append(QPoint(nc, nr));
                }
            }
        }
    }
}

void MoveGenerator::getRookMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        for (int step = 1; step < 10; ++step) {
            int nr = row + dr[i] * step;
            int nc = col + dc[i] * step;
            
            if (!isValidPos(nr, nc)) break;
            
            if (board[nr][nc].isEmpty()) {
                moves.append(QPoint(nc, nr));
            } else {
                if (!isSameSide(board[row][col], board[nr][nc])) {
                    moves.append(QPoint(nc, nr));
                }
                break; // Blocked by a piece, stop sliding
            }
        }
    }
}

void MoveGenerator::getCannonMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        bool foundMount = false;
        
        for (int step = 1; step < 10; ++step) {
            int nr = row + dr[i] * step;
            int nc = col + dc[i] * step;
            
            if (!isValidPos(nr, nc)) break;
            
            if (!foundMount) {
                if (board[nr][nc].isEmpty()) {
                    moves.append(QPoint(nc, nr));
                } else {
                    foundMount = true;
                }
            } else {
                if (!board[nr][nc].isEmpty()) {
                    if (!isSameSide(board[row][col], board[nr][nc])) {
                        moves.append(QPoint(nc, nr));
                    }
                    break; // Captured or blocked by the second piece, stop
                }
            }
        }
    }
}

void MoveGenerator::getPawnMoves(const QVector<QVector<Piece>>& board, int row, int col, bool isRed, QVector<QPoint>& moves)
{
    // Red moves up (-1 row), Black moves down (+1 row)
    int forward = isRed ? -1 : 1;
    bool crossedRiver = isRed ? (row <= 4) : (row >= 5);
    
    // Move forward
    int nr = row + forward;
    if (isValidPos(nr, col) && !isSameSide(board[row][col], board[nr][col])) {
        moves.append(QPoint(col, nr));
    }
    
    // Move sideways only after crossing the river
    if (crossedRiver) {
        if (isValidPos(row, col - 1) && !isSameSide(board[row][col], board[row][col - 1])) {
            moves.append(QPoint(col - 1, row));
        }
        if (isValidPos(row, col + 1) && !isSameSide(board[row][col], board[row][col + 1])) {
            moves.append(QPoint(col + 1, row));
        }
    }
}

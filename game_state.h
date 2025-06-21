#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "move_data.h"
#include <QVector>

class GameState
{
public:
    void reset();
    void applyMove(int index); // set the board to move index
    void addMove(const Move& move);
    void undoLastMove();

    void goToFirst();
    void goToLast();
    void stepBack();
    void stepForward();

    int currentIndex() const;
    int moveCount() const;

    const QVector<Move>& getMoveList() const;

private:
    QVector<Move> moves;
    int current = 0; // index is standing
};

#endif // GAME_STATE_H

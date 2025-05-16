#include "game_state.h"
#include <QDebug>

void GameState::reset()
{
    moves.clear();
    current = 0;
}

void GameState::addMove(const Move& move)
{
    // delete the following steps if in between
    if (current < moves.size())
    {
        moves.resize(current);
    }

    moves.append(move);
    current++;
}

void GameState::undoLastMove()
{
    if (current > 0)
        current--;

}

void GameState::applyMove(int index)
{
    if (index < 0 || index > moves.size())
        return;

    qDebug() << "Apply board to move index" << index;

    current = index;
}

void GameState::goToFirst()
{
    applyMove(0);
}

void GameState::goToLast()
{
    applyMove(moves.size());
}

void GameState::stepBack()
{
    if (current > 0)
        applyMove(current - 1);
}

void GameState::stepForward()
{
    if (current < moves.size())
        applyMove(current + 1);
}

int GameState::currentIndex() const { return current; }

int GameState::moveCount() const { return moves.size(); }

const QVector<Move>& GameState::getMoveList() const { return moves; }

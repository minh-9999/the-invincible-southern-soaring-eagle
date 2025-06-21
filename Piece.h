#ifndef PIECE_H
#define PIECE_H

#include <QString>

enum class PieceType {
    None,
    Pawn, Rook, Knight, Bishop, Advisor, King, Cannon
};

struct Piece
{
    PieceType type = PieceType::None;
    bool isRed = true;

    static Piece empty()
    {
        return Piece();
    }

    bool isEmpty() const
    {
        return type == PieceType::None;
    }

    void setFromString(const QString& str)
    {
        if (str.isEmpty() || str == ".") {
            type = PieceType::None;
            return;
        }

        QChar c = str[0].toUpper();  // get chess character
        isRed = str[0].isUpper();    // uppercase is red, lowercase is black

        if (c == 'K') type = PieceType::King;
        else if (c == 'A') type = PieceType::Advisor;
        else if (c == 'B') type = PieceType::Bishop;
        else if (c == 'N') type = PieceType::Knight;
        else if (c == 'R') type = PieceType::Rook;
        else if (c == 'C') type = PieceType::Cannon;
        else if (c == 'P') type = PieceType::Pawn;
        else type = PieceType::None;  // unknown
    }

    QString toString() const
    {
        if (type == PieceType::None) return ".";

        QChar c;
        switch (type) {
        case PieceType::King:    c = 'K'; break;
        case PieceType::Advisor: c = 'A'; break;
        case PieceType::Bishop:  c = 'B'; break;
        case PieceType::Knight:  c = 'N'; break;
        case PieceType::Rook:    c = 'R'; break;
        case PieceType::Cannon:  c = 'C'; break;
        case PieceType::Pawn:    c = 'P'; break;
        default: return ".";
        }

        return isRed ? QString(c) : QString(c.toLower());
    }
};

#endif // PIECE_H

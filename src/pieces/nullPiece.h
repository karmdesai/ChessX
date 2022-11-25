#ifndef NULLPIECE_H
#define NULLPIECE_H

#include "piece.h"

class NullPiece : public Piece {

  public:
    NullPiece(char name, char color);
    ~NullPiece() = default;

    void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
};

#endif

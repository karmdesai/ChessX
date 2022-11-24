#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {
  public:
    Queen(char name, char color);
    ~Queen() = default;

    void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
};

#endif

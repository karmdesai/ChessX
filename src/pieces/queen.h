#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

class Queen : public Piece {
public:
  Queen(char name, char color);
  ~Queen() = default;

  // clone function
  Queen *clone() override { return new Queen{*this}; }

  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;

  void setPieceAsMoved() override;
};

#endif

#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece {
public:
  Knight(char name, char color);
  ~Knight() = default;

  // clone function
  Knight *clone() override { return new Knight{*this}; }
  
  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;

  void setPieceAsMoved() override;
  void getHasMoved() override;
};

#endif

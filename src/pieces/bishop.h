#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

class Bishop : public Piece {
public:
  Bishop(char name, char color);
  ~Bishop() = default;

  // clone function
  Bishop *clone() override { return new Bishop{*this}; }

  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;

  void setPieceAsMoved() override;
  bool getHasMoved() override;
};

#endif

#ifndef NULLPIECE_H
#define NULLPIECE_H

#include "piece.h"

class NullPiece : public Piece {

public:
  NullPiece(char name, char color);
  ~NullPiece() = default;

  // clone function
  NullPiece *clone() override { return new NullPiece{*this}; }

  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
  void setPieceAsMoved() override;
  bool getHasMoved() override;
};

#endif

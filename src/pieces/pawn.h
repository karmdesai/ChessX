#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

class Pawn : public Piece {
private:
  /* We use this to determine whether or not
      the pawn can move two spaces, and by
      extension, can be en passant captured. */
  bool inStartingPosition;

public:
  Pawn(char name, char color, bool inStartingPosition);
  ~Pawn() = default;

  // clone function
  Pawn *clone() override { return new Pawn{*this}; }

  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
  void setPieceAsMoved() override;
  bool getHasMoved() override;
};

#endif

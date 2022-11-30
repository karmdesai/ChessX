#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

class Rook : public Piece {
private:
  /* We use this to determine whether or not
        the Rook is allowed to castle. */
  bool inStartingPosition;

public:
  Rook(char name, char color, bool inStartingPosition);
  ~Rook() = default;

  // clone function
  Rook *clone() override { return new Rook{*this}; }

  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
  void setPieceAsMoved() override;
  void getHasMoved() override;
};

#endif

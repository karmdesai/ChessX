#ifndef KING_H
#define KING_H

#include "piece.h"

class King : public Piece {
  private:
    /* We use this to determine whether or not
        the King is allowed to castle. */
    bool inStartingPosition;

  public:
    King(char name, char color, bool inStartingPosition);
    ~King() = default;

    void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
};

#endif

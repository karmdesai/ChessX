#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

class Knight : public Piece {
public:
  Knight(char name, char color);
  ~Knight() = default;

  void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
};

#endif
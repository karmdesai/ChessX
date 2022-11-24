#include "piece.h"

class Queen : public Piece {
 public:
  Queen(char name, char color);
  void getAllPossibleMoves(std::pair<char, int> currentPosition);
};

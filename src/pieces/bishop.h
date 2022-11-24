#include "piece.h"

class Bishop : public Piece {
  public:
    Bishop(char name, char color);
    ~Bishop() = default;

    void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
};

#include "piece.h"

class Rook : public Piece {
  public:
    Rook(char name, char color);
    ~Rook() = default;

    void getAllPossibleMoves(std::pair<char, int> currentPosition) override;
};

#include "piece.h"

class Rook : public Piece {
    public:
        Rook(char name, char color);
        void getAllPossibleMoves(std::pair<char, int> currentPosition);
};

#include "piece.h"

class Bishop : public Piece {
    public:
        Bishop(char name, char color);
        void getAllPossibleMoves(std::pair<char, int> currentPosition);
};

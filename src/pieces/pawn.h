#include "piece.h"

class Pawn : public Piece {
    private:
        /* We use this to determine whether or not 
            the pawn can move two spaces, and by 
            extension, can be en passant captured. */
        bool inStartingPosition;

    public:
        Pawn(char name, char color, bool inStartingPosition);
        void getAllPossibleMoves(std::pair<char, int> currentPosition);
};

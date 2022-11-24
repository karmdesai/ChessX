#include "piece.h"

class King : public Piece {
    private:
        /* We use this to determine whether or not 
            the King is allowed to castle. */
        bool inStartingPosition;

    public:
        King(char name, char color, bool inStartingPosition);
        void getAllPossibleMoves(std::pair<char, int> currentPosition);
};

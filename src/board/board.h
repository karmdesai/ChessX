#ifndef BOARD_H
#define BOARD_H

#include "../pieces/piece.h"

class Board {
    private:
        // We have an 8x8 x-y coordinate system.
        // The first index is the x-coordinate (which ranges from 0 to 7).
        //  The first index represents the alphabetical column of the board (a-h).
        // The second index is the y-coordinate (which ranges from 0 to 7).
        Piece* currentBoard[8][8];

    public:
        Board(bool customSetup);
        ~Board() = default;

        // not sure where we are actually supposed to put this?
        friend std::ostream& operator<<(std::ostream& out, const Board* myBoard);

        int convertAlphaToNum(char alpha);

        bool pieceAtPosition(std::pair<char, int> position);

        void parsePossibleMoves(Piece &piece, std::pair<char, int> position);
        void parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position);
};

#endif

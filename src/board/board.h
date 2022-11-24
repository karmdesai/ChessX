#ifndef BOARD_H
#define BOARD_H

#include "../pieces/piece.h"

class Board {
    private:
        Piece* currentBoard[8][8];

    public:
        Board();
        ~Board() = default;

        int convertAlphaToNum(char alpha);

        bool pieceAtPosition(std::pair<char, int> position);

        void parsePossibleMoves(Piece &piece, std::pair<char, int> position);
        void parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position);
};

#endif

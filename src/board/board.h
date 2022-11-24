#ifndef BOARD_H
#define BOARD_H

#include "../pieces/piece.h"

class Board {
    private:
        Piece* currentBoard[8][8];

    public:
        Board();
        ~Board();

        int convertAlphaToNum(char alpha);
};

#endif

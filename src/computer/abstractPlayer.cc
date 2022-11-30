#include "abstractPlayer.h"

#include "../board/board.h"

// constructor
AbstractPlayer::AbstractPlayer(char playerColor, Board *board, bool isComputer)
    : playerColor{playerColor}, board{board}, isComputer{isComputer} {}

// Getters
char AbstractPlayer::getPlayerColor() { return this->playerColor; }

Board *AbstractPlayer::getBoard() { return this->board; }

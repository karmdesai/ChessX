#include "abstractPlayer.h"

#include "../board/board.h"

// constructor
AbstractPlayer::AbstractPlayer(char playerColor, Board *board, bool isAComputer)
    : playerColor{playerColor}, board{board}, isAComputer{isAComputer} {}

// Getters
char AbstractPlayer::getPlayerColor() { return this->playerColor; }

Board *AbstractPlayer::getBoard() { return this->board; }

bool AbstractPlayer::isComputer() { return this->isAComputer; }

#include "abstractPlayer.h"

#include "../board/board.h"

// constructor
AbstractPlayer::AbstractPlayer(char playerColor, Board *board) {
  this->playerColor = playerColor;
  this->board = board;
}

// Getters
char AbstractPlayer::getPlayerColor() { return this->playerColor; }

Board *AbstractPlayer::getBoard() { return this->board; }

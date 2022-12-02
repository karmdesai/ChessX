#include "abstractPlayer.h"

#include <random>

#include "../board/board.h"

// constructor
AbstractPlayer::AbstractPlayer(char playerColor, Board *board, bool isAComputer)
    : playerColor{playerColor}, board{board}, isAComputer{isAComputer} {}

// Getters
char AbstractPlayer::getPlayerColor() { return this->playerColor; }

Board *AbstractPlayer::getBoard() { return this->board; }

bool AbstractPlayer::isComputer() { return this->isAComputer; }

/*
 * return random number between x and y using the Mercenne-Twister generator
 * source:
 * https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
 */
int AbstractPlayer::randomNumber(int x, int y) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> uni(x, y);
  return uni(gen);
}

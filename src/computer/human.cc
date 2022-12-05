#include "human.h"

// constructor
Human::Human(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, false} {}

// calculating next move is a no-op for human, return a random move
std::pair<std::pair<char, int>, std::pair<char, int>>
Human::calculateNextMove() {
  return STALEMATE;  // doesn't matter what we put here, won't be called anyways
}

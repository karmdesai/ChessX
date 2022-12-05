#include "human.h"

// constructor
Human::Human(char playerColor, std::unique_ptr<Board> board) : AbstractPlayer{playerColor, std::move(board), false} {}

// calculating next move is a no-op for human, return a random move
std::pair<std::pair<char, int>, std::pair<char, int>>
Human::calculateNextMove() {
  return std::make_pair(std::make_pair('a', 1), std::make_pair('a', 1));
}

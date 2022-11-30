#include "rook.h"

Rook::Rook(char name, char color, bool inStartingPosition)
    : Piece(5, name, color), inStartingPosition{inStartingPosition} {}

void Rook::getAllPossibleMoves(std::pair<char, int> currentPosition) {
  // clear the vector of previous possible moves
  this->allPossibleMoves.clear();

  // Rook can move horizontally and vertically in all directions.
  int x = currentPosition.first;
  int y = currentPosition.second;

  // move left
  while (x > 'a') {
    x -= 1;
    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  x = currentPosition.first;

  // move right
  while (x < 'h') {
    x += 1;
    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  x = currentPosition.first;

  // move down
  while (y > 1) {
    y -= 1;
    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  y = currentPosition.second;

  // move up
  while (y < 8) {
    y += 1;
    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }
}

void Rook::setPieceAsMoved() {
  inStartingPosition = false;
}

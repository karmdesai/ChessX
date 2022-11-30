#include "queen.h"

Queen::Queen(char name, char color) : Piece(9, name, color) {}

void Queen::getAllPossibleMoves(std::pair<char, int> currentPosition) {
  // clear the vector of previous possible moves
  this->allPossibleMoves.clear();

  // Queen can move horizontally, vertically and diagonally in all directions.

  /* Bishop movement */
  int x = currentPosition.first;
  int y = currentPosition.second;

  // move diagonally left and down
  while (x > 'a' && y > 1) {
    x -= 1;
    y -= 1;

    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  x = currentPosition.first;
  y = currentPosition.second;

  // move diagonally left and up
  while (x > 'a' && y < 8) {
    x -= 1;
    y += 1;

    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  x = currentPosition.first;
  y = currentPosition.second;

  // move diagonally right and up
  while (x < 'h' && y < 8) {
    x += 1;
    y += 1;

    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  x = currentPosition.first;
  y = currentPosition.second;

  // move diagonally right and down
  while (x < 'h' && y > 1) {
    x += 1;
    y -= 1;

    std::pair<char, int> newMove = std::make_pair(x, y);
    this->allPossibleMoves.push_back(newMove);
  }

  /* Rook movement */
  x = currentPosition.first;
  y = currentPosition.second;

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

void Queen::setPieceAsMoved() {
  return;
}

bool Queen::getHasMoved() {
  return;
}

#include "knight.h"

Knight::Knight(char name, char color) : Piece(3, name, color) {}

void Knight::getAllPossibleMoves(std::pair<char, int> currentPosition) {
  // clear the vector of previous possible moves
  this->allPossibleMoves.clear();

  /* Knight can move in an "L" shape in all directions.
      This boils down to 8 unique moves. */

  // move 2 up and 1 right
  std::pair<char, int> newMove =
      std::make_pair(currentPosition.first + 1, currentPosition.second + 2);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 up and 1 left
  newMove =
      std::make_pair(currentPosition.first - 1, currentPosition.second + 2);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 down and 1 right
  newMove =
      std::make_pair(currentPosition.first + 1, currentPosition.second - 2);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 down and 1 left
  newMove =
      std::make_pair(currentPosition.first - 1, currentPosition.second - 2);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 right and 1 up
  newMove =
      std::make_pair(currentPosition.first + 2, currentPosition.second + 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 right and 1 down
  newMove =
      std::make_pair(currentPosition.first + 2, currentPosition.second - 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 left and 1 up
  newMove =
      std::make_pair(currentPosition.first - 2, currentPosition.second + 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move 2 left and 1 down
  newMove =
      std::make_pair(currentPosition.first - 2, currentPosition.second - 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }
}

void Knight::setPieceAsMoved() {
  return;
}

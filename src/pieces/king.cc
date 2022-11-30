#include "king.h"
#include <climits>

// give the King a value of INT_MAX
King::King(char name, char color, bool inStartingPosition)
    : Piece(INT_MAX, name, color), inStartingPosition(inStartingPosition) {}

void King::getAllPossibleMoves(std::pair<char, int> currentPosition) {
  // clear the vector of previous possible moves
  this->allPossibleMoves.clear();

  // the King can long and short castle
  if (this->inStartingPosition) {
    std::pair<char, int> shortCastle =
        std::make_pair(currentPosition.first + 2, currentPosition.second);
    std::pair<char, int> longCastle =
        std::make_pair(currentPosition.first - 2, currentPosition.second);

    // choosing not to check if moves are in bounds
    this->allPossibleMoves.push_back(longCastle);
    this->allPossibleMoves.push_back(shortCastle);
  }

  /* The King can move one space in any direction. Note that
      the comments below that specify the forward/backward/left/right
      directions are for the White king. The Black king moves in the
      opposite directions. */

  // move right one space
  std::pair<char, int> newMove =
      std::make_pair(currentPosition.first + 1, currentPosition.second);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move left one space
  newMove = std::make_pair(currentPosition.first - 1, currentPosition.second);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move forward one space
  newMove = std::make_pair(currentPosition.first, currentPosition.second + 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move backward one space
  newMove = std::make_pair(currentPosition.first, currentPosition.second - 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move forward one space and to the right one space (diagonally)
  newMove =
      std::make_pair(currentPosition.first + 1, currentPosition.second + 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move forward one space and to the left one space (diagonally)
  newMove =
      std::make_pair(currentPosition.first - 1, currentPosition.second + 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move backward one space and to the left one space (diagonally)
  newMove =
      std::make_pair(currentPosition.first - 1, currentPosition.second - 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }

  // move backward one space and to the right one space (diagonally)
  newMove =
      std::make_pair(currentPosition.first + 1, currentPosition.second - 1);
  if (this->isMoveInBounds(newMove)) {
    this->allPossibleMoves.push_back(newMove);
  }
}

void King::setPieceAsMoved() {
  inStartingPosition = false;
}

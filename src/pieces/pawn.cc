#include "pawn.h"
#include <vector>
#include <iostream>

Pawn::Pawn(char name, char color, bool inStartingPosition)
    : Piece(1, name, color), inStartingPosition(inStartingPosition) {}

void Pawn::getAllPossibleMoves(std::pair<char, int> currentPosition) {
  // clear the vector of previous possible moves
  this->allPossibleMoves.clear();

  /* For the pawn specifically, we have to check whether its black
      or white to generate all moves. If its white, we add to the
      y-coordinate. If its black, we subtract from the y-coordinate. */

  // check if the pawn can move two spaces
  if (this->inStartingPosition) {
    if (this->color == 'w') {
      std::pair<char, int> newMove =
          std::make_pair(currentPosition.first, currentPosition.second + 2);

      if (this->isMoveInBounds(newMove)) {
        this->allPossibleMoves.push_back(newMove);
      }
    } else {
      std::pair<char, int> newMove =
          std::make_pair(currentPosition.first, currentPosition.second - 2);

      if (this->isMoveInBounds(newMove)) {
        this->allPossibleMoves.push_back(newMove);
      }
    }
  }

  // the pawn can move one space forward
  if (this->color == 'w') {
    std::pair<char, int> newMove =
        std::make_pair(currentPosition.first, currentPosition.second + 1);

    if (this->isMoveInBounds(newMove)) {
      this->allPossibleMoves.push_back(newMove);
    }
  } else {
    std::pair<char, int> newMove =
        std::make_pair(currentPosition.first, currentPosition.second - 1);

    if (this->isMoveInBounds(newMove)) {
      this->allPossibleMoves.push_back(newMove);
    }
  }

  // the pawn can capture diagonally
  if (this->color == 'w') {
    // move forward and to the right
    std::pair<char, int> diagonalOne =
        std::make_pair(currentPosition.first + 1, currentPosition.second + 1);

    if (this->isMoveInBounds(diagonalOne)) {
      this->allPossibleMoves.push_back(diagonalOne);
    }

    // move forward and to the left
    std::pair<char, int> diagonalTwo =
        std::make_pair(currentPosition.first - 1, currentPosition.second + 1);

    if (this->isMoveInBounds(diagonalTwo)) {
      this->allPossibleMoves.push_back(diagonalTwo);
    }
  } else {
    // move forward and to the left
    std::pair<char, int> diagonalOne =
        std::make_pair(currentPosition.first + 1, currentPosition.second - 1);

    if (this->isMoveInBounds(diagonalOne)) {
      this->allPossibleMoves.push_back(diagonalOne);
    }

    // move forward and to the right
    std::pair<char, int> diagonalTwo =
        std::make_pair(currentPosition.first - 1, currentPosition.second - 1);

    if (this->isMoveInBounds(diagonalTwo)) {
      this->allPossibleMoves.push_back(diagonalTwo);
    }
  }
}

void Pawn::setPieceAsMoved() {
  inStartingPosition = false;
}

bool Pawn::getHasMoved() {
  return;
}


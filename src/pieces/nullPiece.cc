#include "nullPiece.h"

NullPiece::NullPiece(char name, char color) : Piece(0, name, color) {}

void NullPiece::getAllPossibleMoves(std::pair<char, int> currentPosition) {
  this->allPossibleMoves.clear();
}

void NullPiece::setPieceAsMoved() {
  return;
}

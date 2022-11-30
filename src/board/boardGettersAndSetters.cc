#include "../pieces/piece.h"
#include "board.h"

Piece *Board::getPieceAtPosition(std::pair<char, int> position) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  return this->currentBoard[x][y];
}
void Board::setPieceAtPosition(std::pair<char, int> position, Piece *p) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  this->currentBoard[x][y] = p;
}

char Board::getTurn() { return this->whosTurn; }
void Board::setTurn(char player) {
  if (player == 'w' || player == 'b') {
    this->whosTurn = player;
  }
}

Piece *Board::getWhiteKing() { return this->whiteKing; }
void Board::setWhiteKing(Piece *wk) { this->whiteKing = wk; }

Piece *Board::getBlackKing() { return this->blackKing; }
void Board::setBlackKing(Piece *bk) { this->blackKing = bk; }

std::pair<char, int> Board::getWhiteKingPosition() {
  return this->whiteKingPosition;
}
void Board::setWhiteKingPosition(std::pair<char, int> position) {
  this->whiteKingPosition = position;
}

std::pair<char, int> Board::getBlackKingPosition() {
  return this->blackKingPosition;
}
void Board::setBlackKingPosition(std::pair<char, int> position) {
  this->blackKingPosition = position;
}

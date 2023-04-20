#include "board.h"

/* Getters */
// returns a pointer to the piece at the given coordinates
Piece *Board::getPieceAtPosition(std::pair<char, int> position) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  return this->currentBoard[x][y];
}

// returns a pointer to the white king on the board
Piece *Board::getWhiteKing() { return this->whiteKing; }

// returns a pointer to the black king on the board
Piece *Board::getBlackKing() { return this->blackKing; }

// returns the position of the white king on the board
std::pair<char, int> Board::getWhiteKingPosition() {
  return this->whiteKingPosition;
}

// returns the position of the black king on the board
std::pair<char, int> Board::getBlackKingPosition() {
  return this->blackKingPosition;
}

// returns a pointer to the white player AbstractPlayer
AbstractPlayer *Board::getWhitePlayer() { return this->whitePlayer; }

// returns a pointer to the black player AbstractPlayer
AbstractPlayer *Board::getBlackPlayer() { return this->blackPlayer; }

// returns a pointer to the player whose turn it is
AbstractPlayer *Board::getWhosPlayerTurn() { return this->whosPlayerTurn; }

// get the colour of the piece at a given position
char Board::getColourTurn() { return this->whosColourTurn; }

/* Setters */
// sets the piece at a given position to a given piece
void Board::setPieceAtPosition(std::pair<char, int> position, Piece *p) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  this->currentBoard[x][y] = p;
}

// sets who's turn it is
void Board::setColourTurn(char player) {
  if (player == 'w' || player == 'b') {
    this->whosColourTurn = player;
  }
}

// sets the next player whos turn is next
void Board::setPlayerTurn(AbstractPlayer *player) {
  this->whosPlayerTurn = player;
}

// sets the current white king for the board
void Board::setWhiteKing(Piece *wk) { this->whiteKing = wk; }

// sets the current black king for the board
void Board::setBlackKing(Piece *bk) { this->blackKing = bk; }

// sets the position of the white king on the board
void Board::setWhiteKingPosition(std::pair<char, int> position) {
  this->whiteKingPosition = position;
}

// sets the position of the black king on the board
void Board::setBlackKingPosition(std::pair<char, int> position) {
  this->blackKingPosition = position;
}

// sets the white player for the current game
void Board::setWhitePlayer(AbstractPlayer *player) {
  this->whitePlayer = player;
}

// sets the black player for the current game
void Board::setBlackPlayer(AbstractPlayer *player) {
  this->blackPlayer = player;
}

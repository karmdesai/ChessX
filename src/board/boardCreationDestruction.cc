#include "board.h"

#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/nullPiece.h"
#include "../pieces/pawn.h"
#include "../pieces/piece.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"

// Board constructor
Board::Board() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      this->currentBoard[x][y] = new NullPiece('*', '*');
    }
  }
  this->whosColourTurn = 'w';
}

// Board destructor
Board::~Board() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      delete this->currentBoard[x][y];
    }
  }
}

// if 'setup' is not called, we use the default board setup
void Board::defaultInitialization() {
  // Initialize white pieces
  delete this->currentBoard[0][0];
  delete this->currentBoard[1][0];
  delete this->currentBoard[2][0];
  delete this->currentBoard[3][0];
  delete this->currentBoard[4][0];
  delete this->currentBoard[5][0];
  delete this->currentBoard[6][0];
  delete this->currentBoard[7][0];

  this->currentBoard[0][0] = new Rook('R', 'w', true);
  this->currentBoard[1][0] = new Knight('N', 'w');
  this->currentBoard[2][0] = new Bishop('B', 'w');
  this->currentBoard[3][0] = new Queen('Q', 'w');

  Piece *newWhiteKing = new King('K', 'w', true);
  this->whiteKing = newWhiteKing;
  this->currentBoard[4][0] = newWhiteKing;
  this->whiteKingPosition = std::make_pair('e', 1);

  this->currentBoard[5][0] = new Bishop('B', 'w');
  this->currentBoard[6][0] = new Knight('N', 'w');
  this->currentBoard[7][0] = new Rook('R', 'w', true);

  for (int i = 0; i < 8; i++) {
    delete this->currentBoard[i][1];
    this->currentBoard[i][1] = new Pawn('P', 'w', true);
  }

  // Initialize black pieces
  delete this->currentBoard[0][7];
  delete this->currentBoard[1][7];
  delete this->currentBoard[2][7];
  delete this->currentBoard[3][7];
  delete this->currentBoard[4][7];
  delete this->currentBoard[5][7];
  delete this->currentBoard[6][7];
  delete this->currentBoard[7][7];

  this->currentBoard[0][7] = new Rook('r', 'b', true);
  this->currentBoard[1][7] = new Knight('n', 'b');
  this->currentBoard[2][7] = new Bishop('b', 'b');
  this->currentBoard[3][7] = new Queen('q', 'b');

  Piece *newBlackKing = new King('k', 'b', true);
  this->blackKing = newBlackKing;
  this->currentBoard[4][7] = newBlackKing;
  this->blackKingPosition = std::make_pair('e', 8);

  this->currentBoard[5][7] = new Bishop('b', 'b');
  this->currentBoard[6][7] = new Knight('n', 'b');
  this->currentBoard[7][7] = new Rook('r', 'b', true);

  for (int i = 0; i < 8; i++) {
    delete this->currentBoard[i][6];
    this->currentBoard[i][6] = new Pawn('p', 'b', true);
  }
}

// creates a deep copy of the current board and returns a pointer to it
Board *Board::clone() {
  Board *newBoard = new Board();

  for (int x = 0; x < 8; ++x) {
    for (int y = 0; y < 8; ++y) {
      delete newBoard->currentBoard[x][y];
      newBoard->currentBoard[x][y] = this->currentBoard[x][y]->clone();

      // set BlackKing and WhiteKing fields.
      if (newBoard->currentBoard[x][y]->getName() == 'k') {
        newBoard->blackKing = newBoard->currentBoard[x][y];
      } else if (newBoard->currentBoard[x][y]->getName() == 'K') {
        newBoard->whiteKing = newBoard->currentBoard[x][y];
      }
    }
  }

  newBoard->whosColourTurn = this->whosColourTurn;
  newBoard->whiteKingPosition = this->whiteKingPosition;
  newBoard->blackKingPosition = this->blackKingPosition;

  return newBoard;
}

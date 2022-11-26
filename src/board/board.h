#ifndef BOARD_H
#define BOARD_H

#include <ostream>

#include "../pieces/piece.h"

class Board {
private:
  // We have an 8x8 x-y coordinate system.
  // The first index is the x-coordinate (which ranges from 0 to 7).
  //  The first index represents the alphabetical column of the board (a-h).
  // The second index is the y-coordinate (which ranges from 0 to 7).
  Piece *currentBoard[8][8];

  char whosTurn;

public:
  Board();
  ~Board();

  void defaultInitialization();

  // Overloaded operator<< to print board
  friend std::ostream &operator<<(std::ostream &out, const Board *myBoard);

  // Helpers
  int convertAlphaToNum(char alpha);
  char convertNumToAlpha(int num);

  Piece *createPiece(char);

  // Getters
  Piece *getPieceAtPosition(std::pair<char, int> position);

  // Setters
  void setPieceAtPosition(std::pair<char, int> position, Piece *p);
  void setTurn(char player);

  // Move parsers/validators
  void parsePossibleMoves(Piece &piece, std::pair<char, int> position);
  void parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position);
  void parsePossibleMovesQueen(Piece &queen, std::pair<char, int> position);
  void parsePossibleMovesRook(Piece &rook, std::pair<char, int> position);
  void parsePossibleMovesBishop(Piece &bishop, std::pair<char, int> position);

  // Condition managers (check, checkmate, draw, win, etc.)
  bool inCheck(Piece &king);
};

#endif

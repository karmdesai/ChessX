#ifndef PIECE_H
#define PIECE_H

#include <vector>

class Piece {
protected:
  int value;

  // capital letters are white, lowercase are black
  char name;

  // either 'b' or 'w'
  char color;

public:
  // Public for testing purposes (for now), in the future make it protected w/
  // Board as a friend.
  /* Moves are represented by a pair<char, int>, where:
      - the character is a letter ranging from a through h.
      - the integer is a number ranging from 1 to 8.

      Although the character ranges from a through h, we can
      also treat it as an integer ranging from 97 to 104 (ASCII). */
  std::vector<std::pair<char, int>> allPossibleMoves;

public:
  // Constructor and destructor
  Piece(int value, char name, char color);
  virtual ~Piece() = default;

  // To deep copy a piece
  virtual Piece* clone() = 0;

  int getValue();
  char getName();
  char getColor();

  // used to check whether or not a move is in the 8x8 bounds
  bool isMoveInBounds(std::pair<char, int> move);

  // the Piece class is an abstract class
  void virtual getAllPossibleMoves(std::pair<char, int> currentPosition) = 0;
  void virtual setPieceAsMoved() = 0;
};

#endif

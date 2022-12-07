#include "board.h"

#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/nullPiece.h"
#include "../pieces/pawn.h"
#include "../pieces/piece.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"

#include <ostream>

// Overload the << operator for the Board class
std::ostream &operator<<(std::ostream &out, const Board *myBoard) {
  for (int y = 7; y >= 0; y--) {
    out << y + 1 << " ";

    for (int x = 0; x < 8; x++) {
      Piece *currentSpace = myBoard->currentBoard[x][y];

      if (currentSpace->getName() != '*') {
        out << currentSpace->getName();
      } else {
        // a even sum of coordinates is a black square
        if ((x + y) % 2 == 0) {
          out << "_";
        }
        // an odd sum of coordinates is a white square
        else {
          out << " ";
        }
      }
    }

    out << std::endl;
  }

  // print x-axis as letters
  out << std::endl;
  out << "  abcdefgh";
  out << std::endl;

  return out;
}

// convertAlphaToNum() converts a character ranging from a to h
//  to a number ranging from 0 to 7.
int Board::convertAlphaToNum(char alpha) {
  if (alpha >= 'a' && alpha <= 'h') {
    return alpha - 'a';
  } else {
    return -1;
  }
}

// convertNumToAlpha() converts a number ranging from 0 to 7
//  to a character ranging from a to h.
char Board::convertNumToAlpha(int num) {
  if (num >= 0 && num <= 7) {
    return num + 'a';
  } else {
    return '*';
  }
}

// createPiece(c) takes a character c and creates a new Piece
//  of name c. It returns a pointer to the newly created piece.
Piece *Board::createPiece(char c) {
  if (c == 'R') {
    return new Rook('R', 'w', false);
  } else if (c == 'N') {
    return new Knight('N', 'w');
  } else if (c == 'B') {
    return new Bishop('B', 'w');
  } else if (c == 'Q') {
    return new Queen('Q', 'w');
  } else if (c == 'K') {
    return new King('K', 'w', false);
  } else if (c == 'P') {
    return new Pawn('P', 'w', false);
  } else if (c == 'r') {
    return new Rook('r', 'b', false);
  } else if (c == 'n') {
    return new Knight('n', 'b');
  } else if (c == 'b') {
    return new Bishop('b', 'b');
  } else if (c == 'q') {
    return new Queen('q', 'b');
  } else if (c == 'k') {
    return new King('k', 'b', false);
  } else if (c == 'p') {
    return new Pawn('p', 'b', false);
  } else {
    return new NullPiece('*', '*');
  }
}
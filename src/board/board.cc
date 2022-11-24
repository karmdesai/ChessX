#include "board.h"

#include <iostream>
#include <vector>
#include "../pieces/piece.h"

/* Board class */
Board::Board() {}

// convertAlphaToNum() converts a character ranging from a to h
//  to a number ranging from 0 to 7.
// requires:
//  - alpha is a character ranging from a to h.
// time: O(1)
int Board::convertAlphaToNum(char alpha) {
    if (alpha >= 'a' && alpha <= 'h') {
        return alpha - 'a' + 1;
    } else {
        return -1;
    }
}

bool Board::pieceAtPosition(std::pair<char, int> position) {
    int x = convertAlphaToNum(position.first);
    int y = position.second;

    /* Check if position is out of bounds first?

    if (x == -1 || y < 1 || y > 8) {
        return false;
    }
    */

    if (currentBoard[x][y] == nullptr) {
        return false;
    } else {
        return true;
    }
}

void Board::parsePossibleMoves(Piece &piece, std::pair<char, int> position) {
  if (piece.getName() == 'p' || piece.getName() == 'P') {
    // Pawn
  } else if (piece.getName() == 'r' || piece.getName() == 'R') {
    // Rook
  } else if (piece.getName() == 'n' || piece.getName() == 'N') {
    parsePossibleMovesKnight(piece, position);
  } else if (piece.getName() == 'b' || piece.getName() == 'B') {
    // Bishop
  } else if (piece.getName() == 'q' || piece.getName() == 'Q') {
    // Queen
  } else if (piece.getName() == 'k' || piece.getName() == 'K') {
    // King
  }
}

void Board::parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : knight.allPossibleMoves) {
    /* the knight can jump other pieces, so all possible moves are valid as
     long as they are not occupied. */
    if (this->pieceAtPosition(move) == false) {
      tmp.push_back(move);
    }
  }

  knight.allPossibleMoves = tmp;
}

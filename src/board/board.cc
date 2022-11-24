#include "board.h"

#include "../pieces/piece.h"

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

Board::Board() {}

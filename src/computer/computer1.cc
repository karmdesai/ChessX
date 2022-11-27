#include "computer1.h"

#include "../board/board.h"
#include "../pieces/piece.h"
#include <iostream>

// the constructor for Computer1
Computer1::Computer1(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board} {}

// calculate the next move
std::pair<char, int> Computer1::calculateNextMove() {
  /*
  Steps:
    - Save ALL possible moves for our color to a list
    - Randomly choose one (for level 1)
  */

  // list to keep track of moves
  std::vector<std::pair<char, int>> moves;

  // loop through the board
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // // get the piece at the current position
      // Piece *p =
      //     this->board->getPieceAtPosition(std::make_pair(i + 'a', j + 1));

      // // if the piece is not null and it is our color
      // if (p != nullptr && p->getColor() == this->playerColor) {
      //   // get all possible moves for the piece
      //   p->getAllPossibleMoves(std::make_pair(i + 'a', j + 1));

      //   // loop through all possible moves
      //   for (auto move : p->allPossibleMoves) {
      //     // add the move to the list
      //     moves.push_back(move);
      //   }
      // }
      std::cout << "i: " << i + 'a' << " j: " << j+1 << std::endl;
    }
  }
}

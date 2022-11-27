#include "computer1.h"

#include <iostream>
#include <random>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer1
Computer1::Computer1(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board} {}

// return random number between x and y using the Mercenne-Twister generator
// source:
// https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
int randomNumber(int x, int y) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> uni(x, y);
  return uni(gen);
}

// calculate the next move
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer1::calculateNextMove() {
  /*
  Steps:
    - Save ALL possible moves for our color to a list
    - Randomly choose one (for level 1)
  */

  // list to keep track of moves, using a vector of pairs of pairs
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>> moves;

  // loop through the board
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // get the piece at the current position
      auto currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = board->getPieceAtPosition(currentPos);

      // if the piece is not null and it is our color
      if (p->getName() != '*' && p->getName() != 'k' && p->getName() != 'K' && p->getColor() == this->playerColor) {
        // get all possible moves for the piece
        p->getAllPossibleMoves(std::make_pair(char(i + 'a'), j + 1));
        board->parsePossibleMoves(*p, currentPos);

        // loop through all possible moves
        for (auto move : p->allPossibleMoves) {
          // add the move to the list
          auto theMove = std::make_pair(currentPos, move);
          // print the move
          std::cout << "move: " << theMove.first.first << theMove.first.second
                    << " to " << theMove.second.first << theMove.second.second
                    << std::endl;
          moves.push_back(theMove);
        }
      }
    }
  }

  // from the list of moves, randomly choose one
  int randomIndex = randomNumber(0, moves.size() - 1);
  auto randomMove = moves.at(randomIndex);
  return randomMove;
}

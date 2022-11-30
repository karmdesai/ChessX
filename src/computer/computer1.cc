#include "computer1.h"

#include <iostream>
#include <random>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer1
Computer1::Computer1(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, true} {}

/*
 * return random number between x and y using the Mercenne-Twister generator
 * source:
 * https://stackoverflow.com/questions/5008804/generating-a-random-integer-from-a-range
 */
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
    * if the king of the player is in check, we can only make moves that
      get the king out of check
    * if the king is not in check, we can make any move
  */

  // list to keep track of moves, using a vector of pairs of pairs
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>> allMoves;

  // loop through the board to get all the possible moves
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // get the piece at the current position
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = board->getPieceAtPosition(currentPos);

      // if the piece is not nullPiece and it is our color
      if (p->getName() != '*' && p->getColor() == this->playerColor) {
        // get all possible moves for the piece
        p->getAllPossibleMoves(std::make_pair(char(i + 'a'), j + 1));
        board->parsePossibleMoves(*p, currentPos);

        // add moves to the list
        for (auto move : p->allPossibleMoves) {
          auto theMove = std::make_pair(currentPos, move);
          allMoves.push_back(theMove);
        }
      }
    }
  }

  // if the king is in check, we can only make moves that get the king out of
  // check

  std::pair<char, int> kingPos;
  Piece *king;
  // get the king's position
  if (this->playerColor == 'w') {
    kingPos = board->getWhiteKingPosition();
    king = board->getWhiteKing();
  } else {
    kingPos = board->getBlackKingPosition();
    king = board->getBlackKing();
  }

  // if the king is in check
  if (board->inCheck(*king, kingPos)) {
    // list of moves that get the king out of check
    std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
        movesThatGetKingOutOfCheck;

    // loop through all the moves
    for (auto move : allMoves) {
      std::cout << "Move being checked: " << move.first.first
                << move.first.second << " to " << move.second.first
                << move.second.second << std::endl;
      // make the move on a copy of the board
      Board *boardCopy = board->clone();
      bool success = boardCopy->movePiece(move.first, move.second);
      if (success) {
        movesThatGetKingOutOfCheck.push_back(move);
      } else {
        std::cout << "Move failed" << std::endl;
      }
      // // if the king is not in check, add the move to the list
      // if (!boardCopy->inCheck(*king, kingPos)) {
      //   std::cout << "got here for move: " << move.first.first
      //             << move.first.second << " to " << move.second.first
      //             << move.second.second << std::endl;
      //   movesThatGetKingOutOfCheck.push_back(move);
      // }

      // delete the copy of the board
      delete boardCopy;
    }

    // if there are moves that get the king out of check, return one of them
    if (movesThatGetKingOutOfCheck.size() > 0) {
      std::cout << "number of possible moves: "
                << movesThatGetKingOutOfCheck.size() << std::endl;
      std::cout << "-----------------" << std::endl;
      for (auto move : movesThatGetKingOutOfCheck) {
        std::cout << move.first.first << move.first.second << " -> "
                  << move.second.first << move.second.second << std::endl;
      }
      std::cout << "-----------------" << std::endl;
      int randomIndex = randomNumber(0, movesThatGetKingOutOfCheck.size() - 1);
      return movesThatGetKingOutOfCheck[randomIndex];
    }
  }

  // if the king is not in check, return a move that does not put its king in
  // check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatDoNotPutKingInCheck;

  // loop through all the moves
  for (auto move : allMoves) {
    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    boardCopy->movePiece(move.first, move.second);

    // if the king is not in check, add the move to the list
    if (!boardCopy->inCheck(*king, kingPos)) {
      movesThatDoNotPutKingInCheck.push_back(move);
    }

    // delete the copy of the board
    delete boardCopy;
  }

  std::cout << "-----------------" << std::endl;
  for (auto move : allMoves) {
    std::cout << move.first.first << move.first.second << " -> "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "-----------------" << std::endl;

  // if the king is not in check, we can make any move
  int randomIndex = randomNumber(0, allMoves.size() - 1);
  auto randomMove = allMoves.at(randomIndex);
  std::cout << "Computer plays: " << randomMove.first.first
            << randomMove.first.second << " -> " << randomMove.second.first
            << randomMove.second.second << std::endl;
  return randomMove;
}

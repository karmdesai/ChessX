#include "computer1.h"

#include <iostream>
#include <algorithm>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer1
Computer1::Computer1(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, true} {}

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
      // make the move on a copy of the board
      Board *boardCopy = board->clone();
      bool success = boardCopy->movePiece(move.first, move.second);
      if (success) {
        movesThatGetKingOutOfCheck.push_back(move);
      }

      // delete the copy of the board
      delete boardCopy;
    }

    // if there are moves that get the king out of check, return one of them
    if (movesThatGetKingOutOfCheck.size() > 0) {
      int randomIndex = randomNumber(0, movesThatGetKingOutOfCheck.size() - 1);
      return movesThatGetKingOutOfCheck[randomIndex];
    } else {
      // we've lost, as there are no moves that get the king out of check
      return std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));
    }
  }

  /// remove moves from allMoves that put the king in check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatDoNotPutOurKingInCheck;

  // loop through all the moves
  for (auto move : allMoves) {
    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    bool success = boardCopy->movePiece(move.first, move.second);

    // if the king is not in check, add the move to the list
    if (success) {
      movesThatDoNotPutOurKingInCheck.push_back(move);
    }

    // delete the copy of the board
    delete boardCopy;
  }

  // if the king is not in check, we can make any move
  if (allMoves.size() > 0) {
    int randomIndex = randomNumber(0, allMoves.size() - 1);
    return allMoves[randomIndex];
  } else {
    // it's a stalemate, as we're not in check and there are no moves
    return std::make_pair(std::make_pair('a', 0), std::make_pair('a', 0));
  }
}

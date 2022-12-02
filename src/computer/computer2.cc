#include "computer2.h"

#include <iostream>
#include <algorithm>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer1
Computer2::Computer2(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, true} {}

// calculate the next move, returns a pair of moves
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer2::calculateNextMove() {
  /* This version of the computer prefers capturing moves and moves that put the
  opponent in check. If there are multiple moves that fit these criteria, it
  will choose one at random. If there are no moves that fit these criteria, it
  will choose a random move like Computer1. */

  // first get all the possible moves
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

  // if king is in check, then we must first get out of check
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
    } else {  // we've lost :(
      return std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));
    }
  }

  // remove moves from allMoves that put the king in check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatDoNotPutOurKingInCheck;

  // loop through all the moves
  for (auto move : allMoves) {
    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    bool success = boardCopy->movePiece(move.first, move.second);

    // if the king is not in check, add the move to the list
    if (success && !boardCopy->inCheck(*king, kingPos)) {
      movesThatDoNotPutOurKingInCheck.push_back(move);
    }

    // delete the copy of the board
    delete boardCopy;
  }

  // get all moves from movesThatDoNotPutOurKingInCheck that capture a piece
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatCapture;

  // loop through all the moves
  for (auto move : movesThatDoNotPutOurKingInCheck) {
    // get the piece at the destination
    Piece *p = board->getPieceAtPosition(move.second);

    // if the piece is not nullPiece, add the move to the list
    if (p->getName() != '*' && p->getColor() != this->playerColor) {
      movesThatCapture.push_back(move);
    }
  }

  // get all moves that put the opponent in check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatPutOpponentInCheck;

  // loop through all the moves
  for (auto move : movesThatDoNotPutOurKingInCheck) {
    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    boardCopy->movePieceBase(move.first, move.second);

    Piece *king;
    std::pair<char, int> kingPos;

    // get the opponent's king's position
    if (this->playerColor == 'w') {
      kingPos = boardCopy->getBlackKingPosition();
      king = boardCopy->getBlackKing();
    } else {
      kingPos = boardCopy->getWhiteKingPosition();
      king = boardCopy->getWhiteKing();
    }

    // if the king is in check, add the move to the list
    if (boardCopy->inCheck(*king, kingPos)) {
      movesThatPutOpponentInCheck.push_back(move);
    }

    // delete the copy of the board
    delete boardCopy;
  }

  // best case is that there is a move that captures a piece and puts the
  // opponent in check. If such a move (or moves) exist, return one of them

  // get intersection of movesThatCapture and movesThatPutOpponentInCheck
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatCaptureAndPutOpponentInCheck;

  // using std::set_intersection
  // source:
  // https://stackoverflow.com/questions/19483663/vector-intersection-in-c
  std::set_intersection(
      movesThatCapture.begin(), movesThatCapture.end(),
      movesThatPutOpponentInCheck.begin(), movesThatPutOpponentInCheck.end(),
      std::back_inserter(movesThatCaptureAndPutOpponentInCheck));

  if (movesThatCaptureAndPutOpponentInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatCaptureAndPutOpponentInCheck.size() - 1);
    return movesThatCaptureAndPutOpponentInCheck[randomIndex];
  }

  // if there's no intersecting moves, we just choose a random move from the
  // union of movesThatCapture and movesThatPutOpponentInCheck

  // get union of movesThatCapture and movesThatPutOpponentInCheck
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatCaptureOrPutOpponentInCheck;

  // using std::set_union
  std::set_union(movesThatCapture.begin(), movesThatCapture.end(),
                 movesThatPutOpponentInCheck.begin(),
                 movesThatPutOpponentInCheck.end(),
                 std::back_inserter(movesThatCaptureOrPutOpponentInCheck));

  if (movesThatCaptureOrPutOpponentInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatCaptureOrPutOpponentInCheck.size() - 1);
    return movesThatCaptureOrPutOpponentInCheck[randomIndex];
  }

  // if there are no moves that capture or put the opponent in check, we just
  // choose a random move from movesThatDoNotPutOurKingInCheck

  if (movesThatDoNotPutOurKingInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatDoNotPutOurKingInCheck.size() - 1);
    return movesThatDoNotPutOurKingInCheck[randomIndex];
  }

  // if there are no moves AT ALL its a stalemate, as the king isn't in check
  // (otherwise we would have returned a move that gets the king out of check
  // or would have realized that we've been checkmated)
  return std::make_pair(std::make_pair('a', 0), std::make_pair('a', 0));
}

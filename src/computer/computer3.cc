#include "computer3.h"

#include <random>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer3
Computer3::Computer3(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board} {}

// calculate the next move, returns a pair of moves
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer3::calculateNextMove() {
  /* This version of the computer prefers avoiding capture first, capturing
  moves and then checking the opponent. If there are multiple possible moves
  that fix the criteria, then it will choose one at random. If there are no
  moves that fit this criteria at all, computer chooses a random move. */

  /* Steps:
    1. Get all possible moves
    2. Filter out the moves that don't capture the opponent's piece
    3. Filter out the moves that don't put the opponent in check
    4. Filter out the moves that don't avoid capture
    5. Choose a random move from the remaining moves
  */

  // Step 1: get all the possible moves
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>> moves;

  // loop through the board
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // get the piece at the current position
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = board->getPieceAtPosition(currentPos);

      // if the piece is not nullPiece and it is our color
      // For now WE DON'T SUPPORT King moves as kingParser is not implemented
      if (p->getName() != '*' && p->getName() != 'k' && p->getName() != 'K' &&
          p->getColor() == this->playerColor) {
        // get all possible moves for the piece
        p->getAllPossibleMoves(std::make_pair(char(i + 'a'), j + 1));
        board->parsePossibleMoves(*p, currentPos);

        // add moves to the list
        for (auto move : p->allPossibleMoves) {
          auto theMove = std::make_pair(currentPos, move);
          moves.push_back(theMove);
        }
      }
    }
  }

  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      preferredMoves;

  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      preferredMovesLikeComputerTwo;

  // Step 2: filter out the moves that don't capture the opponent's piece
  for (auto move : moves) {
    // get the piece at the destination
    Piece *p = board->getPieceAtPosition(move.second);

    // if the piece is not nullPiece and it is the opponent's color
    if (p->getName() != '*' && p->getColor() != this->playerColor) {
      // add the move to the list
      preferredMoves.push_back(move);
      preferredMovesLikeComputerTwo.push_back(move);
    }
  }

  // Step 3: filter out the moves that don't put the opponent in check

  /* This cannot be done as the check function is not implemented.
  But, the idea is to make a copy of the board (using the board->clone()
  function), make the move on the copy, and then check if the opponent is in
  check. If they are, add the move to the list. The function will also need to
  make sure that the move does not put the computer itself in check. */

  // Step 4: filter out the moves that don't avoid capture
  // for each move, check if the destination can be captured by the opponent
  // if it can, don't add it to the list
  // if it can't for all opponent pieces, add it to the list

  for (auto move : moves) {
    // get the piece at the destination
    Piece *p = board->getPieceAtPosition(move.second);

    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    boardCopy->makeMove(move.first, move.second);

    bool badMove = false;

    // iterate through each opponent piece, and check if it can capture the
    // piece at the destination

    // loop through the board
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
        Piece *oppPiece = boardCopy->getPieceAtPosition(currentPos);

        // if the piece is nullPiece or it is our color, skip
        if (oppPiece->getName() == '*' ||
            oppPiece->getColor() == this->playerColor) {
          continue;
        }

        // the piece must be an opponent's piece
        else {
          // get all possible legal moves for the piece
          oppPiece->getAllPossibleMoves(currentPos);
          boardCopy->parsePossibleMoves(*oppPiece, currentPos);

          // check if the destination is in the list of possible moves
          if (std::find(oppPiece->allPossibleMoves.begin(),
                        oppPiece->allPossibleMoves.end(),
                        move.second) != oppPiece->allPossibleMoves.end()) {
            // the destination can be captured by the opponent, so don't add
            // the move to the list
            badMove = true;
            break;
          }
        }
      }
    }
    if (!badMove) {
      preferredMoves.push_back(move);
    }

    // delete the copy of the board
    delete boardCopy;
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  // choose a random move from preferredMoves if it is not empty
  if (preferredMoves.size() > 0) {
    std::uniform_int_distribution<> uni(0, preferredMoves.size() - 1);
    int randomIndex = uni(gen);
    return preferredMoves[randomIndex];
  }

  // otherwise, choose a random move from preferredMovesLikeComputerTwo if it is
  // not empty
  else if (preferredMovesLikeComputerTwo.size() > 0) {
    std::uniform_int_distribution<> uni(
        0, preferredMovesLikeComputerTwo.size() - 1);
    int randomIndex = uni(gen);
    return preferredMovesLikeComputerTwo[randomIndex];
  }

  // else, return a random move from moves
  else {
    std::uniform_int_distribution<> uni(0, moves.size() - 1);
    int randomIndex = uni(gen);
    return moves[randomIndex];
  }
}

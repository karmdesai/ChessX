#include "computer2.h"

#include <iostream>
#include <random>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer1
Computer2::Computer2(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board} {}

// calculate the next move, returns a pair of moves
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer2::calculateNextMove() {
  /* This version of the computer prefers capturing moves and moves that put the
  opponent in check. If there are multiple moves that fit these criteria, it
  will choose one at random. If there are no moves that fit these criteria, it
  will choose a random move like Computer1. */

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

  // Step 2: filter out the moves that don't capture the opponent's piece

  for (auto move : moves) {
    // get the piece at the destination
    Piece *p = board->getPieceAtPosition(move.second);

    // if the piece is not nullPiece and it is the opponent's color
    if (p->getName() != '*' && p->getColor() != this->playerColor) {
      // add the move to the list
      preferredMoves.push_back(move);
    }
  }

  // Step 3: filter out the moves that don't put the opponent in check

  /* This cannot be done as the check function is not implemented.
  But, the idea is to make a copy of the board (using the board->clone()
  function), make the move on the copy, and then check if the opponent is in
  check. If they are, add the move to the list. The function will also need to
  make sure that the move does not put the computer itself in check. */

  // Step 4: choose a random move from preferredMoves if there are any
  std::random_device rd;
  std::mt19937 gen(rd());

  // print all moves
  std::cout << "All moves: " << std::endl;
  for (auto move : moves) {
    std::cout << move.first.first << move.first.second << " -> "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------" << std::endl;

  // print preferred moves
  std::cout << "Preferred moves: " << std::endl;
  for (auto move : preferredMoves) {
    std::cout << move.first.first << move.first.second << " -> "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------" << std::endl;

  if (preferredMoves.size() > 0) {
    // choose a random move from the list
    std::uniform_int_distribution<> dis(0, preferredMoves.size() - 1);
    int randomIndex = dis(gen);

    return preferredMoves.at(randomIndex);
  } else {
    // no preferred moves exist, choose a random legal move
    std::uniform_int_distribution<> dis(0, moves.size() - 1);
    int randomIndex = dis(gen);

    return moves.at(randomIndex);
  }
}

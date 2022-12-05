#include "computer4.h"

#include <climits>
#include <iostream>
#include <vector>

#include "../board/board.h"
#include "../pieces/piece.h"
#include "computer1.h"
#include "pieceTables.h"

/*
We want to make this version of the computer much more intelligent
than the previous versions. To do this, we will implement a minimax
algorithm with alpha-beta pruning. This will allow us to search
through the game tree and find the best move for the computer.

In order for minimax to work, we need to be able to evaluate the
board at any given state. Computer's like Stockfish use a very
complex evaluation function, but for the sake of time and simplicity,
we will use a much simpler evaluation function. We don't want it
to be too simple though. Here's a list of all the things we want
our evaluation function to take into account:

1. Total value of all the pieces (using the centipawn method) on the board, and
finding the delta between the two players
2. Placement of pieces depending on the stage of the game (opening/midgame and
endgame)
3. Whether it is checkmate or stalemate

*/

const auto CHECKMATED =
    std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));

const int MAX_DEPTH = 1;

// the constructor for Computer4
Computer4::Computer4(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, true} {}

// evaluate function
int evaluate(Board *b, char playerColor) {
  // if playerColor is checkmated, immediately return highest/lowest
  Computer1 c1{playerColor, b};
  auto move = c1.calculateNextMove();
  if (move == CHECKMATED) {
    if (playerColor == 'w') {
      return INT_MIN;
    } else {
      return INT_MAX;
    }
  }

  char otherPlayerColor = (playerColor == 'w') ? 'b' : 'w';

  // if playerColor checkmates the opponent, immediately return highest/lowest
  Computer1 c2{otherPlayerColor, b};
  move = c2.calculateNextMove();
  if (move == CHECKMATED) {
    if (playerColor == 'w') {
      return INT_MAX;
    } else {
      return INT_MIN;
    }
  }

  int totalWhitePieceValue = 0, totalBlackPieceValue = 0, whitePieces = 0,
      blackPieces = 0;

  int delta;

  // for each of playerColor's pieces, add the piece value to the total
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = b->getPieceAtPosition(currentPos);

      if (p->getColor() == 'w') {
        totalWhitePieceValue += 100 * p->getValue();  // centipawn value
        ++whitePieces;
      } else if (p->getColor() == 'b') {
        totalBlackPieceValue += 100 * p->getValue();  // centipawn value
        ++blackPieces;
      }
    }
  }

  // if playerColor is white
  if (playerColor == 'w') {
    delta = totalWhitePieceValue - totalBlackPieceValue;
  } else {  // if playerColor is black
    delta = totalBlackPieceValue - totalWhitePieceValue;
  }

  int openingAndMiddleGame = 0, endGame = 0;

  double howMuchAreWeInOpeningAndMiddleGame =
      double(whitePieces + blackPieces) / double(32);
  double howMuchAreWeInEndGame = 1 - howMuchAreWeInOpeningAndMiddleGame;

  // add values to delta based on piece tables for our pieces
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = b->getPieceAtPosition(currentPos);

      if (p->getColor() == playerColor) {
        if (char(tolower(p->getName()) == 'k')) {
          if (playerColor == 'w') {
            openingAndMiddleGame += howMuchAreWeInOpeningAndMiddleGame *
                                    kingWhiteTableOpening[i][j];
            endGame += howMuchAreWeInEndGame * kingWhiteTableEndgame[i][j];
          } else {
            openingAndMiddleGame += howMuchAreWeInOpeningAndMiddleGame *
                                    kingBlackTableOpening[i][j];
            endGame += howMuchAreWeInEndGame * kingBlackTableEndgame[i][j];
          }
        } else if (char(tolower(p->getName()) == 'q')) {
          openingAndMiddleGame +=
              howMuchAreWeInOpeningAndMiddleGame * queenTable[i][j];
          endGame += howMuchAreWeInEndGame * queenTable[i][j];
        } else if (char(tolower(p->getName()) == 'r')) {
          if (playerColor == 'w') {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * rookWhiteTable[i][j];
            endGame += howMuchAreWeInEndGame * rookWhiteTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * rookBlackTable[i][j];
            endGame += howMuchAreWeInEndGame * rookBlackTable[i][j];
          }
        } else if (char(tolower(p->getName()) == 'b')) {
          openingAndMiddleGame +=
              howMuchAreWeInOpeningAndMiddleGame * bishopTable[i][j];
          endGame += howMuchAreWeInEndGame * bishopTable[i][j];
        } else if (char(tolower(p->getName()) == 'n')) {
          openingAndMiddleGame +=
              howMuchAreWeInOpeningAndMiddleGame * knightTable[i][j];
          endGame += howMuchAreWeInEndGame * knightTable[i][j];
        } else if (char(tolower(p->getName()) == 'p')) {
          if (playerColor == 'w') {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * pawnWhiteTable[i][j];
            endGame += howMuchAreWeInEndGame * pawnWhiteTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * pawnBlackTable[i][j];
            endGame += howMuchAreWeInEndGame * pawnBlackTable[i][j];
          }
        }
      }
    }
  }

  delta += int((openingAndMiddleGame * howMuchAreWeInOpeningAndMiddleGame) +
               (endGame * howMuchAreWeInEndGame));

  if (playerColor == 'w')
    return delta;
  else
    return -delta;
}

// minimax function
int minimax(Board *b, int depth, int alpha, int beta, bool isMaximizingPlayer, char playerColor) {
  if (depth == 0) {
    return evaluate(b, playerColor);
  }

  if (isMaximizingPlayer) {
    int maximumEvaluation = INT_MIN;
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
        Piece *p = b->getPieceAtPosition(currentPos);

        if (p->getColor() == playerColor) {
          p->getAllPossibleMoves(currentPos);
          b->parsePossibleMoves(*p, currentPos);

          for (auto move : p->allPossibleMoves) {
            Board *bCopy = b->clone();
            bCopy->movePiece(currentPos, move);
            int evaluation = minimax(bCopy, depth - 1, alpha, beta, false, playerColor);
            maximumEvaluation = std::max(maximumEvaluation, evaluation);

            delete bCopy;
          }
        }
      }
      return maximumEvaluation;
    }
  } else {
    int minimumEvaluation = INT_MAX;
    for (int i = 0; i < 8; ++i) {
      for (int j = 0; j < 8; ++j) {
        std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
        Piece *p = b->getPieceAtPosition(currentPos);

        if (p->getColor() != playerColor && p->getColor() != ' ') {
          p->getAllPossibleMoves(currentPos);
          b->parsePossibleMoves(*p, currentPos);

          for (auto move : p->allPossibleMoves) {
            Board *bCopy = b->clone();
            bCopy->movePiece(currentPos, move);
            int evaluation = minimax(bCopy, depth - 1, alpha, beta, true, playerColor);
            minimumEvaluation = std::min(minimumEvaluation, evaluation);
            delete bCopy;
          }
        }
      }
    }
    return minimumEvaluation;
  }
  return 0;
}

// function to calculate the next move
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer4::calculateNextMove() {
  // we run minimax for every possible move and choose the one with the highest
  // evaluation
  std::cout << "Computer is thinking..." << std::endl;
  int eval;
  if (playerColor == 'w')
    eval = INT_MIN;
  else
    eval = INT_MAX;

  auto bestMove =
      std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = board->getPieceAtPosition(currentPos);

      if (p->getColor() == playerColor) {
        p->getAllPossibleMoves(currentPos);
        board->parsePossibleMoves(*p, currentPos);

        for (auto move : p->allPossibleMoves) {
          Board *boardCopy = board->clone();
          boardCopy->movePiece(currentPos, move);
          int evaluation;
          if (playerColor == 'w') {
            evaluation = minimax(boardCopy, MAX_DEPTH, INT_MIN, INT_MAX, true, playerColor);
            if (evaluation > eval) {
              eval = evaluation;
              bestMove = std::make_pair(currentPos, move);
            }
          } else {
            evaluation = minimax(boardCopy, MAX_DEPTH, INT_MIN, INT_MAX, false, playerColor);
            if (evaluation < eval) {
              eval = evaluation;
              bestMove = std::make_pair(currentPos, move);
            }
          }
          if (playerColor == 'w') {
            if (evaluation > eval) {
              eval = evaluation;
              bestMove = std::make_pair(currentPos, move);
            }
          } else {
            if (evaluation < eval) {
              eval = evaluation;
              bestMove = std::make_pair(currentPos, move);
            }
          }
          delete boardCopy;
        }
      }
    }
  }

  return bestMove;
}

#include "computer4.h"

#include <climits>
#include <iostream>
#include <vector>

#include "../board/board.h"
#include "../pieces/piece.h"
#include "computer1.h"

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

INSPIRATION:
* https://www.chessprogramming.org/Evaluation
* https://www.chessprogramming.org/Simplified_Evaluation_Function
  * (we used the piece-tables from this, thank you Tomasz Michniewski!)
*/

// piece-table for the king (opening/midgame)
// in the opening and midgame, we prefer the king in the corners, protected by
// its pawns and other pieces
const int kingTableOpening[8][8] = {{-30, -40, -40, -50, -50, -40, -40, -30},
                                    {-30, -40, -40, -50, -50, -40, -40, -30},
                                    {-30, -40, -40, -50, -50, -40, -40, -30},
                                    {-30, -40, -40, -50, -50, -40, -40, -30},
                                    {-20, -30, -30, -40, -40, -30, -30, -20},
                                    {-10, -20, -20, -20, -20, -20, -20, -10},
                                    {20, 20, 0, 0, 0, 0, 20, 20},
                                    {20, 30, 10, 0, 0, 10, 30, 20}};

// piece-table for the king (endgame)
// in the endgame, we prefer the king in the center of the board
const int kingTableEndgame[8][8] = {{-50, -40, -30, -20, -20, -30, -40, -50},
                                    {-30, -20, -10, 0, 0, -10, -20, -30},
                                    {-30, -10, 20, 30, 30, 20, -10, -30},
                                    {-30, -10, 30, 40, 40, 30, -10, -30},
                                    {-30, -10, 30, 40, 40, 30, -10, -30},
                                    {-30, -10, 20, 30, 30, 20, -10, -30},
                                    {-30, -30, 0, 0, 0, 0, -30, -30},
                                    {-50, -30, -30, -30, -30, -30, -30, -50}};

// piece-table for the queen (opening/midgame/endgame)
// we prefer the queen in the center of the board
const int queenTable[8][8] = {{-20, -10, -10, -5, -5, -10, -10, -20},
                              {-10, 0, 0, 0, 0, 0, 0, -10},
                              {-10, 0, 5, 5, 5, 5, 0, -10},
                              {-5, 0, 5, 5, 5, 5, 0, -5},
                              {0, 0, 5, 5, 5, 5, 0, -5},
                              {-10, 5, 5, 5, 5, 5, 0, -10},
                              {-10, 0, 5, 0, 0, 0, 0, -10},
                              {-20, -10, -10, -5, -5, -10, -10, -20}};

// piece-table for the rook (opening/midgame/endgame)
// we prefer the rook on the open files
const int rookTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},   {5, 10, 10, 10, 10, 10, 10, 5},
    {-5, 0, 0, 0, 0, 0, 0, -5}, {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5}, {-5, 0, 0, 0, 0, 0, 0, -5},
    {-5, 0, 0, 0, 0, 0, 0, -5}, {3, 0, 0, 5, 5, 0, 0, 3}};

// piece-table for the bishop (opening/midgame/endgame)
// we prefer the bishop in the center to look in all directions
const int bishopTable[8][8] = {{-20, -10, -10, -10, -10, -10, -10, -20},
                               {-10, 0, 0, 0, 0, 0, 0, -10},
                               {-10, 0, 5, 10, 10, 5, 0, -10},
                               {-10, 5, 5, 10, 10, 5, 5, -10},
                               {-10, 0, 10, 10, 10, 10, 0, -10},
                               {-10, 10, 10, 10, 10, 10, 10, -10},
                               {-10, 5, 0, 0, 0, 0, 5, -10},
                               {-20, -10, -10, -10, -10, -10, -10, -20}};

// piece-table for the knight (opening/midgame/endgame)
// we prefer the knight in the center to look in all directions
const int knightTable[8][8] = {{-50, -40, -30, -30, -30, -30, -40, -50},
                               {-40, -20, 0, 0, 0, 0, -20, -40},
                               {-30, 0, 10, 15, 15, 10, 0, -30},
                               {-30, 5, 15, 20, 20, 15, 5, -30},
                               {-30, 0, 15, 20, 20, 15, 0, -30},
                               {-30, 5, 10, 15, 15, 10, 5, -30},
                               {-40, -20, 0, 5, 5, 0, -20, -40},
                               {-50, -40, -30, -30, -30, -30, -40, -50}};

// piece-table for the pawn (opening/midgame/endgame)
// we prefer the pawns up the board so that they can be promoted and attack
// the enemy king
const int pawnTable[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},         {50, 50, 50, 50, 50, 50, 50, 50},
    {10, 20, 30, 30, 30, 30, 20, 10}, {10, 10, 20, 20, 20, 10, 10, 10},
    {5, 5, 10, 25, 25, 10, 5, 5},     {5, 5, 5, 5, 5, 7, 7, 7},
    {0, -5, -10, -20, -20, 5, 5, 5},  {0, 0, 0, 0, 0, 0, 0, 0}};

const auto CHECKMATED =
    std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));

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

  bool useNewPieceTables = false;

  // if player is black, we need to flip the piece tables
  int **newKingTableOpening;
  int **newKingTableEndGame;
  int **newQueenTable;
  int **newRookTable;
  int **newBishopTable;
  int **newKnightTable;
  int **newPawnTable;

  if (playerColor == 'b') {
    useNewPieceTables = true;
    newKingTableOpening = flipTable(kingTableOpening);
    newKingTableEndGame = flipTable(kingTableEndgame);
    newQueenTable = flipTable(queenTable);
    newRookTable = flipTable(rookTable);
    newBishopTable = flipTable(bishopTable);
    newKnightTable = flipTable(knightTable);
    newPawnTable = flipTable(pawnTable);
  }

  // add values to delta based on piece tables for our pieces
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = b->getPieceAtPosition(currentPos);

      if (p->getColor() == playerColor) {
        if (char(tolower(p->getName()) == 'k')) {
          if (useNewPieceTables) {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * newKingTableOpening[i][j];
            endGame +=
                howMuchAreWeInEndGame * newKingTableEndGame[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * kingTableOpening[i][j];
            endGame += howMuchAreWeInEndGame * kingTableEndgame[i][j];
          }
        } else if (char(tolower(p->getName()) == 'q')) {
          if (useNewPieceTables) {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * newQueenTable[i][j];
            endGame += howMuchAreWeInEndGame * newQueenTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * queenTable[i][j];
            endGame += howMuchAreWeInEndGame * queenTable[i][j];
          }
        } else if (char(tolower(p->getName()) == 'r')) {
          if (useNewPieceTables) {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * newRookTable[i][j];
            endGame += howMuchAreWeInEndGame * newRookTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * rookTable[i][j];
            endGame += howMuchAreWeInEndGame * rookTable[i][j];
          }
        } else if (char(tolower(p->getName()) == 'b')) {
          if (useNewPieceTables) {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * newBishopTable[i][j];
            endGame += howMuchAreWeInEndGame * newBishopTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * bishopTable[i][j];
            endGame += howMuchAreWeInEndGame * bishopTable[i][j];
          }
        } else if (char(tolower(p->getName()) == 'n')) {
          if (useNewPieceTables) {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * newKnightTable[i][j];

            endGame += howMuchAreWeInEndGame * newKnightTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * knightTable[i][j];

            endGame += howMuchAreWeInEndGame * knightTable[i][j];
          }
        } else if (char(tolower(p->getName()) == 'p')) {
          if (useNewPieceTables) {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * newPawnTable[i][j];
            endGame += howMuchAreWeInEndGame * newPawnTable[i][j];
          } else {
            openingAndMiddleGame +=
                howMuchAreWeInOpeningAndMiddleGame * pawnTable[i][j];
            endGame += howMuchAreWeInEndGame * pawnTable[i][j];
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

// function to turn piece table from white's perspective to black's perspective
int **flipTable(const int table[8][8]) {
  int **tableFlipped = new int *[8];
  for (int i = 0; i < 8; i++) {
    tableFlipped[i] = new int[8];
    for (int j = 0; j < 8; j++) {
      tableFlipped[i][j] = table[7 - i][7 - j];
    }
  }
  return tableFlipped;
}

// minimax function
int minimax(Board *b, int depth, bool isMaximizingPlayer, char playerColor) {
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
            int evaluation = minimax(bCopy, depth - 1, false, playerColor);
            maximumEvaluation = std::max(maximumEvaluation, evaluation);
            delete bCopy;
          }
          return maximumEvaluation;
        }
      }
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
            int evaluation = minimax(bCopy, depth - 1, true, playerColor);
            minimumEvaluation = std::min(minimumEvaluation, evaluation);
            delete bCopy;
          }
        }
        return minimumEvaluation;
      }
    }
  }
}

// function to calculate the next move
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer4::calculateNextMove() {
  // TODO: implement this
}

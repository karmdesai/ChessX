#include "computer4.h"

#include <iostream>
#include <vector>

#include "../board/board.h"
#include "../pieces/piece.h"

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

// the constructor for Computer4
Computer4::Computer4(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, true} {}

// evaluate function
int evaluate(Board *b, char playerColor) {
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
        if (tolower(p->getName()) == 'k') {
          openingAndMiddleGame += kingTableOpening[i][j];
          endGame += kingTableEndgame[i][j];
        } else if (tolower(p->getName()) == 'q') {
          openingAndMiddleGame += queenTable[i][j];
          endGame += queenTable[i][j];
        } else if (tolower(p->getName()) == 'r') {
          openingAndMiddleGame += rookTable[i][j];
          endGame += rookTable[i][j];
        } else if (tolower(p->getName()) == 'b') {
          openingAndMiddleGame += bishopTable[i][j];
          endGame += bishopTable[i][j];
        } else if (tolower(p->getName()) == 'n') {
          openingAndMiddleGame += knightTable[i][j];
          endGame += knightTable[i][j];
        } else if (tolower(p->getName()) == 'p') {
          openingAndMiddleGame += pawnTable[i][j];
          endGame += pawnTable[i][j];
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
  // TODO: implement this
}

// function to calculate the next move
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer4::calculateNextMove() {
  // TODO: implement this
}

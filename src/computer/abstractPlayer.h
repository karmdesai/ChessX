#ifndef ABSTRACT_PLAYER_H
#define ABSTRACT_PLAYER_H

#include <vector>

class Board;

class AbstractPlayer {
 protected:
  char playerColor;
  Board *board;

 public:
  AbstractPlayer(char playerColor, Board *board);
  virtual ~AbstractPlayer() = default;

  // Getters
  char getPlayerColor();
  Board *getBoard();

  // calculate the next move, returns a pair of moves
  virtual std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove() = 0;
};

#endif

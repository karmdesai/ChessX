#ifndef ABSTRACT_PLAYER_H
#define ABSTRACT_PLAYER_H

#include <vector>

class Board;

class AbstractPlayer {
 protected:
  char playerColor;
  bool isComputer;
  Board *board;

 public:
  AbstractPlayer(char playerColor, Board *board, bool isComputer);
  virtual ~AbstractPlayer() = default;

  // Getters
  char getPlayerColor();
  Board *getBoard();
  bool getIsComputer();

  // calculate the next move, returns a pair of moves
  virtual std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove() = 0;
};

#endif

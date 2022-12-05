#ifndef ABSTRACT_PLAYER_H
#define ABSTRACT_PLAYER_H

#include <vector>
#include <memory>
#include "../board/board.h"

class AbstractPlayer {
 protected:
  char playerColor;
  std::unique_ptr<Board> board;
  bool isAComputer;

 public:
  friend class Board;
  AbstractPlayer(char playerColor, std::unique_ptr<Board> board, bool isAComputer);
  virtual ~AbstractPlayer() = default;

  // Getters
  char getPlayerColor();
  std::unique_ptr<Board> getBoard();
  bool isComputer();

  // calculate the next move, returns a pair of moves
  virtual std::pair<std::pair<char, int>, std::pair<char, int>>
  calculateNextMove() = 0;

  // random number generator
  int randomNumber(int x, int y);
};

#endif

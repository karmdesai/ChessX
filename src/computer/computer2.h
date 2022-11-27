#ifndef COMPUTER_TWO_H
#define COMPUTER_TWO_H

#include <vector>

#include "abstractPlayer.h"

class Computer2 : public AbstractPlayer {
 public:
  Computer2(char playerColor, Board *board);
  ~Computer2() = default;

  // calculate the next move, returns a pair of moves
  std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove()
      override;
};

#endif

#ifndef COMPUTER_ONE_H
#define COMPUTER_ONE_H

#include "abstractPlayer.h"
#include <vector>

class Computer1 : public AbstractPlayer {
 public:
  Computer1(char playerColor, Board *board);
  ~Computer1() = default;

  // calculate the next move
  std::pair<char, int> calculateNextMove();
};

#endif

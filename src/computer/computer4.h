#ifndef COMPUTER_FOUR_H
#define COMPUTER_FOUR_H

#include <vector>

#include "abstractPlayer.h"

class Computer4 : public AbstractPlayer {
 public:
  Computer4(char playerColor, Board *board);
  ~Computer4() = default;

  // calculate the next move, returns a pair of moves
  std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove()
      override;
};

#endif

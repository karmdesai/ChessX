#ifndef COMPUTER_ONE_H
#define COMPUTER_ONE_H

#include <vector>

#include "abstractPlayer.h"

class Computer1 : public AbstractPlayer {
 public:
  Computer1(char playerColor, std::unique_ptr<Board> board);
  ~Computer1() = default;

  // calculate the next move, returns a pair of moves
  std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove()
      override;
};

#endif

#ifndef COMPUTER_THREE_H
#define COMPUTER_THREE_H

#include <vector>

#include "abstractPlayer.h"

class Computer3 : public AbstractPlayer {
 public:
  Computer3(char playerColor, std::unique_ptr<Board> board);
  ~Computer3() = default;

  // calculate the next move, returns a pair of moves
  std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove()
      override;
};

#endif

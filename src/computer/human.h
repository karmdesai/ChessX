#ifndef HUMAN_H
#define HUMAN_H

#include "abstractPlayer.h"

class Human : public AbstractPlayer {
 public:
  Human(char playerColor, std::unique_ptr<Board> board);
  ~Human() = default;

  // calculate the next move, returns a pair of moves
  std::pair<std::pair<char, int>, std::pair<char, int>> calculateNextMove() override;
};

#endif

#ifndef COMPUTER_ONE_H
#define COMPUTER_ONE_H

#include "abstractPlayer.h"

class Computer1 : public AbstractPlayer {
 public:
  Computer1(char playerColor, Board *board);
  ~Computer1() = default;
};

#endif

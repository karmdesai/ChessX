#ifndef STUDIO_H
#define STUDIO_H

#include "subject.h"
#include <iostream>
#include <algorithm>
#include "piece.h"

class Board;

class Studio: public Subject {
  std::ostream &out = std::cout;

  Board *thePicture;

 public:
  explicit Studio(Board *board): thePicture{board} {}

  Board *&picture() { return thePicture; }
  void reset();
  void render();
  Piece* getState(std::pair<char, int> coord)const;
  ~Studio();
};

#endif
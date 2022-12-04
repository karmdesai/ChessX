#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include "../pieces/piece.h"

class Observer;

class Subject {
  std::vector<Observer*> observers;
 public:
  Subject();
  void attach(Observer *o);
  void detach(Observer *o);
  void notifyObservers(std::pair<char, int> from, std::pair<char, int> to);
  virtual Piece* getState(std::pair<char, int> coord) const = 0;
  virtual ~Subject() = default;
};

#endif

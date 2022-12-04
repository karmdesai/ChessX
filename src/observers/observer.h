#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include <algorithm>

class Subject;

class Observer {
 public:
  virtual void notify(std::pair<char, int> from, std::pair<char, int> to) = 0;
  virtual ~Observer() = default;
};
#endif

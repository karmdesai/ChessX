#ifndef _OBSERVER_H_
#define _OBSERVER_H_
class Subject;

class Observer {
 public:
  virtual void notify(std::pair<char, int> before, std::pair<char, int> after) = 0;
  virtual ~Observer() = default;
};
#endif

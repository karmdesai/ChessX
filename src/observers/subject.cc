#include "subject.h"
#include "observer.h"
#include <iostream>
#include <algorithm>

Subject::Subject() {}

void Subject::attach( Observer *o ) {
  observers.emplace_back(o);
}

void Subject::detach( Observer *o ) {
  for ( auto it = observers.begin(); it != observers.end(); ++it ) {
    if ( *it == o ) {
      observers.erase(it);
      break;
    }
  }
}

void Subject::notifyObservers() {
  for (auto ob: observers) {
    ob->notify();
  }
}

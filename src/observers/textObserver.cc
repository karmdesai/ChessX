#include "observer.h"
#include "studio.h"
#include <iostream>
#include "textObserver.h"
using namespace std;

TextObs::TextObs(Studio *canvas): canvas{canvas} {
    canvas->attach(this);
}

TextObs::~TextObs() {
  canvas->detach(this);
}

void TextObs::notify(std::pair<char, int> before, std::pair<char, int> after) {
    
}

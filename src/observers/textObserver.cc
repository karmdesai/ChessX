#include "observer.h"
#include "studio.h"
#include <iostream>
#include "textObserver.h"

TextObs::TextObs(Studio *canvas): canvas{canvas} {
    canvas->attach(this);
}

TextObs::~TextObs() {
  canvas->detach(this);
}

void TextObs::notify(std::pair<char, int>, std::pair<char, int>, bool) {
  for (int y = 8; y > 0; y--) {
    std::cout << y << " ";

    for (int x = 0; x < 8; x++) {
      Piece *currentSpace = canvas->getState(std::make_pair(x + 'a', y));

      if (currentSpace->getName() != '*') {
        std::cout << currentSpace->getName();
      } else {
        // a even sum of coordinates is a black square
        if ((x + y) % 2 == 0) {
          std::cout << "_";
        }
        // an odd sum of coordinates is a white square
        else {
          std::cout << " ";
        }
      }
    }

    std::cout << std::endl;
  }

  // print x-axis as letters
  std::cout << std::endl;
  std::cout << "  abcdefgh";
  std::cout << std::endl;

}

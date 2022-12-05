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

void TextObs::notify(std::pair<char, int> from, std::pair<char, int> to, bool enPassant) {
  for (int y = 8; y > 0; y--) {
    cout << y << " ";

    for (int x = 0; x < 8; x++) {
      Piece *currentSpace = canvas->getState(make_pair(x + 'a', y));

      if (currentSpace->getName() != '*') {
        cout << currentSpace->getName();
      } else {
        // a even sum of coordinates is a black square
        if ((x + y) % 2 == 0) {
          cout << "_";
        }
        // an odd sum of coordinates is a white square
        else {
          cout << " ";
        }
      }
    }

    cout << std::endl;
  }

  // print x-axis as letters
  cout << std::endl;
  cout << "  abcdefgh";
  cout << std::endl;

}

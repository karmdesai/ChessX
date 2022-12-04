#include "studio.h"
#include <algorithm>
#include "board.h"
#include "piece.h"

void Studio::render(std::pair<char, int> from, std::pair<char, int> to, bool enPassant) {
  notifyObservers(from, to, enPassant);
}

Studio::~Studio() { delete thePicture; }

Piece* Studio::getState(std::pair<char, int> coord) const {
  return thePicture->getPieceAtPosition(coord);
}

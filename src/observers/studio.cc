#include "studio.h"
#include <algorithm>
#include "board.h"
#include "piece.h"

void Studio::reset() {}

void Studio::render() {
  notifyObservers();
}

Studio::~Studio() { delete thePicture; }

Piece* Studio::getState(std::pair<char, int> coord) const {
  return thePicture->pieceAt(coord);
}

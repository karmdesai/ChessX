#include "piece.h"
#include <vector>

Piece::Piece(int value, char name, char color, bool inStartingPosition)
    : value(value), name(name), color(color),
      inStartingPosition(inStartingPosition) {}

int Piece::getValue() { return value; }
char Piece::getName() { return name; }
char Piece::getColor() { return color; }

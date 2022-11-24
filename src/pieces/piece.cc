#include "piece.h"
#include <vector>

Piece::Piece(int value, char name, char color)
    : value(value), name(name), color(color) {
    this->allPossibleMoves = std::vector<std::pair<char, int>>();
}

int Piece::getValue() { return value; }
char Piece::getName() { return name; }
char Piece::getColor() { return color; }

bool Piece::isMoveInBounds(std::pair<char, int> move) {
    if (move.first < 'a' || move.first > 'h' || 
        move.second < 1 || move.second > 8) {
        return false;
    }

    return true;
}

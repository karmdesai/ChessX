#include "board.h"

// sets whether an en passant pawn is possible
void Board::setEnPassantPawn(std::pair<char, int> pawn) {
  this->enPassantPawn.first = pawn.first;
  this->enPassantPawn.second = pawn.second;
}

// helper functions for observers
bool Board::getEnPassantMade() { return enPassantMade; }
void Board::setEnPassantFalse() { enPassantMade = false; }

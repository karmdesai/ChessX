#include "computer1.h"

// constructor
Computer1::Computer1(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board} {}

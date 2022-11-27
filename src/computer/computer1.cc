#include "computer1.h"

// the constructor for Computer1
Computer1::Computer1(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board} {}

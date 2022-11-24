#include "board.h"

// convertAlphaToNum() converts a character ranging from a to h
//  or A to H to a number ranging from 0 to 7.
// requires: 
//  - alpha is a character ranging from a to h or A to H.
// time: O(1)
int Board::convertAlphaToNum(char alpha) {
    if (alpha >= 'a' && alpha <= 'h') {
        return alpha - 'a' + 1;
    } else if (alpha >= 'A' && alpha <= 'H') {
        return alpha - 'A' + 1;
    } else {
        return -1;
    }
}

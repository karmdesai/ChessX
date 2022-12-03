#include "computer4.h"

#include <vector>
#include <iostream>

#include "../board/board.h"
#include "../pieces/piece.h"

/*
We want to make this version of the computer much more intelligent
than the previous versions. To do this, we will implement a minimax
algorithm with alpha-beta pruning. This will allow us to search
through the game tree and find the best move for the computer.

In order for minimax to work, we need to be able to evaluate the
board at any given state. Computer's like Stockfish use a very
complex evaluation function, but for the sake of time and simplicity,
we will use a much simpler evaluation function. We don't want it 
to be too simple though. Here's a list of all the things we want
our evaluation function to take into account:

1. Total value of all the pieces on the board, and finding the difference
between the two players. This will be the most important factor in our
2. Placement of pieces depending on the stage of the game


*/

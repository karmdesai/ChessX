#include "board.h"

#include <iostream>
#include <vector>

#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/pawn.h"
#include "../pieces/piece.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"

/* Board class */
Board::Board(bool customSetup) {
    // initialize board to nullptr
    for (int j = 0; j < 8; j++) {
        for (int k = 0; k < 8; k++) {
            currentBoard[j][k] = nullptr;
        }
    }

    if (customSetup == false) {

        // Initialize white pieces
        currentBoard[0][0] = new Rook('R', 'w', true);
        currentBoard[1][0] = new Knight('N', 'w');
        currentBoard[2][0] = new Bishop('B', 'w');
        currentBoard[3][0] = new Queen('Q', 'w');
        currentBoard[4][0] = new King('K', 'w', true);
        currentBoard[5][0] = new Bishop('B', 'w');
        currentBoard[6][0] = new Knight('N', 'w');
        currentBoard[7][0] = new Rook('R', 'w', true);

        for (int i = 0; i < 8; i++) {
            currentBoard[i][1] = new Pawn('P', 'w', true);
        }

        // Initialize black pieces
        currentBoard[0][7] = new Rook('r', 'b', true);
        currentBoard[1][7] = new Knight('n', 'b');
        currentBoard[2][7] = new Bishop('b', 'b');
        currentBoard[3][7] = new Queen('q', 'b');
        currentBoard[4][7] = new King('k', 'b', true);
        currentBoard[5][7] = new Bishop('b', 'b');
        currentBoard[6][7] = new Knight('n', 'b');
        currentBoard[7][7] = new Rook('r', 'b', true);

        for (int i = 0; i < 8; i++) {
            currentBoard[i][6] = new Pawn('p', 'b', true);
        }
    }
}

// convertAlphaToNum() converts a character ranging from a to h
//  to a number ranging from 0 to 7.
// requires:
//  - alpha is a character ranging from a to h.
// time: O(1)
int Board::convertAlphaToNum(char alpha) {
    if (alpha >= 'a' && alpha <= 'h') {
        return alpha - 'a';
    } else {
        return -1;
    }
}

bool Board::pieceAtPosition(std::pair<char, int> position) {
    int x = convertAlphaToNum(position.first);
    int y = position.second;

    /* Check if position is out of bounds first?

    if (x == -1 || y < 1 || y > 8) {
        return false;
    }
    */

    if (currentBoard[x][y] == nullptr) {
        return false;
    } else {
        return true;
    }
}

void Board::parsePossibleMoves(Piece &piece, std::pair<char, int> position) {
    if (piece.getName() == 'p' || piece.getName() == 'P') {
        // Pawn
    } else if (piece.getName() == 'r' || piece.getName() == 'R') {
        // Rook
    } else if (piece.getName() == 'n' || piece.getName() == 'N') {
        parsePossibleMovesKnight(piece, position);
    } else if (piece.getName() == 'b' || piece.getName() == 'B') {
        // Bishop
    } else if (piece.getName() == 'q' || piece.getName() == 'Q') {
        // Queen
    } else if (piece.getName() == 'k' || piece.getName() == 'K') {
        // King
    }
}

void Board::parsePossibleMovesKnight(Piece &knight,
                                     std::pair<char, int> position) {
    std::vector<std::pair<char, int>> tmp;

    for (auto move : knight.allPossibleMoves) {
        /* the knight can jump other pieces, so all possible moves are valid as
         long as they are not occupied. */
        if (this->pieceAtPosition(move) == false) {
            tmp.push_back(move);
        }
    }

    knight.allPossibleMoves = tmp;
}

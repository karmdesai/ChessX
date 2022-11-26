#include <iostream>

#include "board/board.h"
#include "computer/computer.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/piece.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

int main() {
    Board *b = new Board(false);

    std::cout << "**** WELCOME TO CHESS ****" << std::endl;
    std::cout << b << std::endl;

    std::cout
        << "Enter a piece followed by current position to get all possible "
           "moves: "
        << std::endl;
    std::cout << "e.g. 'R a1' for white rook currently at a1" << std::endl;
    std::cout << "e.g. 'b h4' for black bishop currently at h4" << std::endl;

    Piece *p = nullptr;

    char piece;
    char x;
    int y;

    std::cin >> piece >> x >> y;

    if (x < 'a' || x > 'h' || y < 1 || y > 8) {
        std::cout << "Invalid position" << std::endl;
        return 1;
    }

    std::pair<char, int> position = std::make_pair(x, y);

    if (piece == 'p') {
        bool inStartingPosition = true;

        if (y != 7) {
            inStartingPosition = false;
        }

        p = new Pawn(piece, 'b', inStartingPosition);
        p->getAllPossibleMoves(position);
        b->parsePossibleMoves(*p, position);

    } else if (piece == 'r') {
        bool inStartingPosition = false;

        if ((x == 'a' && y == 8) || (x == 'h' && y == 8)) {
            inStartingPosition = true;
        }

        p = new Rook(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(position);

    } else if (piece == 'n') {
        p = new Knight(piece, 'b');

        p->getAllPossibleMoves(position);
        b->parsePossibleMoves(*p, position);

    } else if (piece == 'b') {
        p = new Bishop(piece, 'b');
        p->getAllPossibleMoves(position);

    } else if (piece == 'q') {
        p = new Queen(piece, 'b');
        p->getAllPossibleMoves(position);

    } else if (piece == 'k') {
        bool inStartingPosition = true;

        if (x != 'e' || y != 8) {
            inStartingPosition = false;
        }

        p = new King(piece, 'b', inStartingPosition);
        p->getAllPossibleMoves(position);

    } else if (piece == 'P') {
        bool inStartingPosition = true;

        if (y != 2) {
            inStartingPosition = false;
        }

        p = new Pawn(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(position);
        b->parsePossibleMoves(*p, position);

    } else if (piece == 'R') {
        bool inStartingPosition = false;

        if ((x == 'a' && y == 1) || (x == 'h' && y == 1)) {
            inStartingPosition = true;
        }

        p = new Rook(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(position);

    } else if (piece == 'N') {
        p = new Knight(piece, 'w');

        p->getAllPossibleMoves(position);
        b->parsePossibleMoves(*p, position);

    } else if (piece == 'B') {
        p = new Bishop(piece, 'w');
        p->getAllPossibleMoves(position);

    } else if (piece == 'Q') {
        p = new Queen(piece, 'w');
        p->getAllPossibleMoves(position);

    } else if (piece == 'K') {
        bool inStartingPosition = true;

        if (x != 'e' || y != 1) {
            inStartingPosition = false;
        }

        p = new King(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(position);

    } else {
        std::cout << "Invalid piece" << std::endl;
    }

    if (p != nullptr) {
        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        std::cout << std::endl;

        delete p;
    }

    delete b;
}

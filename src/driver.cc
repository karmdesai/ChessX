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
    std::cout
        << "Enter a piece followed by current position to get all possible "
           "moves: "
        << std::endl;
    std::cout << "e.g. 'R a1' for white rook currently at a1" << std::endl;
    std::cout << "e.g. 'b h4' for black bishop currently at h4" << std::endl;

    char piece;
    char x;
    int y;

    std::cin >> piece >> x >> y;

    if (piece == 'p') {
        bool inStartingPosition = true;

        if (y != 7) {
            inStartingPosition = false;
        }

        Piece *p = new Pawn('p', 'b', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'r') {
        Piece *p = new Rook('r', 'b');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'n') {
        Piece *p = new Knight('n', 'b');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'b') {
        Piece *p = new Bishop('b', 'b');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'q') {
        Piece *p = new Queen('q', 'b');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'k') {
        bool inStartingPosition = true;

        if (x != 'e' || y != 8) {
            inStartingPosition = false;
        }

        Piece *p = new King('K', 'w', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'P') {
        bool inStartingPosition = true;

        if (y != 2) {
            inStartingPosition = false;
        }

        Piece *p = new Pawn('p', 'b', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'R') {
        Piece *p = new Rook('R', 'w');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'N') {
        Piece *p = new Knight('N', 'w');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'B') {
        Piece *p = new Bishop('B', 'w');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'Q') {
        Piece *p = new Queen('Q', 'w');
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else if (piece == 'K') {
        bool inStartingPosition = true;

        if (x != 'e' || y != 1) {
            inStartingPosition = false;
        }

        Piece *p = new King('K', 'w', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

        for (auto move : p->allPossibleMoves) {
            std::cout << move.first << move.second << ", ";
        }

        delete p;
    } else {
        std::cout << "Invalid piece" << std::endl;
    }
}

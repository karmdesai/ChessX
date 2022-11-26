#include <iostream>

#include "board/board.h"
#include "computer/computer.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/nullPiece.h"
#include "pieces/pawn.h"
#include "pieces/piece.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

int main() {
    std::cout << "**** WELCOME TO CHESS ****" << std::endl;

    std::cout << "Please enter the setup command immediately if you would like "
                 "to use a custom setup."
              << std::endl;
    std::cout << "You will not be able to use a custom setup later on."
              << std::endl;
    std::cout << std::endl;

    // you can only setup on the very first command for now.
    std::string firstCommand;
    std::cin >> firstCommand;

    Board *b = new Board();

    /* Start Board Setup */
    if (firstCommand == "setup") {
        std::cout << b << std::endl;

        std::string setupCommand;

        while (std::cin >> setupCommand) {
            if (setupCommand == "done") {
                break;
            } else if (setupCommand == "+") {
                char piece;
                char x;
                int y;

                std::cin >> piece >> x >> y;
                std::pair<char, int> position = std::make_pair(x, y);

                // if there's already a piece, they need to remove it first.
                if (b->getPieceAtPosition(position)->getName() != '*') {
                    std::cout << "There is already a piece at this position."
                              << std::endl;
                    std::cout << "Remove the piece first or select a different "
                                 "position."
                              << std::endl;
                } else {
                    // no pawns allowed on first/last row.
                    if ((piece == 'p' || piece == 'P') && (y == 0 || y == 7)) {
                        std::cout << "You cannot put a pawn on the first or "
                                     "last rank."
                                  << std::endl;
                        std::cout << "Please select a different position."
                                  << std::endl;
                    } else {
                        Piece *newPiece = b->createPiece(piece);

                        // if they give an invalid character, just delete the
                        // returned Piece.
                        if (newPiece->getName() == '*') {
                            delete newPiece;
                        } else {
                            b->setPieceAtPosition(position, newPiece);
                        }

                        std::cout << b << std::endl;
                    }
                }
            } else if (setupCommand == "-") {
                char x;
                int y;

                std::cin >> x >> y;
                std::pair<char, int> position = std::make_pair(x, y);

                if (b->getPieceAtPosition(position)->getName() != '*') {
                    delete b->getPieceAtPosition(position);
                    b->setPieceAtPosition(position, new NullPiece('*', '*'));

                    std::cout << b << std::endl;
                }
            } else if (setupCommand == "=") {
                char nextPlayer;

                std::cin >> nextPlayer;

                b->setTurn(nextPlayer);
            }
        }

        int whiteKings = 0;
        int blackKings = 0;
        std::pair<char, int> position;

        // Check if the Board has a valid setup.
        for (int x = 0; x < 8; x++) {
            for (int y = 0; y < 8; y++) {
                // convert 0 to 7 to a to h for x-coordinate
                position = std::make_pair(x + 'a', y);

                Piece* currentPiece = b->getPieceAtPosition(position);
                
                if (currentPiece->getName() == 'K') {
                    // if the King is in check, invalid setup.
                    if (b->inCheck(*(currentPiece))) {
                        std::cout << "The King should not be in check."
                                  << std::endl;
                        std::cout << "Error, exiting setup and quitting program."
                                  << std::endl;

                        return 1;
                    }

                    whiteKings += 1;
                } else if (currentPiece->getName() == 'k') {
                    // if the King is in check, invalid setup.
                    if (b->inCheck(*(currentPiece))) {
                        std::cout << "The King should not be in check."
                                  << std::endl;
                        std::cout << "Error, exiting setup and quitting program."
                                  << std::endl;

                        return 1;
                    }

                    blackKings += 1;
                }
            }
        }

        if ((blackKings != 1) || (whiteKings != 1)) {
            std::cout << "There should be exactly 1 white King and 1 black King."
                      << std::endl;
            std::cout << "Error, exiting setup and quitting program."
                      << std::endl;

            return 1;
        }
    } else {
        b->defaultInitialization();
    }

    std::cout << b << std::endl;
    /* End Board Setup */

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

    if (piece == 'p') {
        bool inStartingPosition = true;

        if (y != 7) {
            inStartingPosition = false;
        }

        p = new Pawn(piece, 'b', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'r') {
        bool inStartingPosition = false;

        if ((x == 'a' && y == 8) || (x == 'h' && y == 8)) {
            inStartingPosition = true;
        }

        p = new Rook(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'n') {
        p = new Knight(piece, 'b');
        std::pair<char, int> position = std::make_pair(x, y);

        p->getAllPossibleMoves(position);
        b->parsePossibleMoves(*p, position);

    } else if (piece == 'b') {
        p = new Bishop(piece, 'b');
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'q') {
        p = new Queen(piece, 'b');
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'k') {
        bool inStartingPosition = true;

        if (x != 'e' || y != 8) {
            inStartingPosition = false;
        }

        p = new King(piece, 'b', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'P') {
        bool inStartingPosition = true;

        if (y != 2) {
            inStartingPosition = false;
        }

        p = new Pawn(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'R') {
        bool inStartingPosition = false;

        if ((x == 'a' && y == 1) || (x == 'h' && y == 1)) {
            inStartingPosition = true;
        }

        p = new Rook(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'N') {
        p = new Knight(piece, 'w');
        std::pair<char, int> position = std::make_pair(x, y);

        p->getAllPossibleMoves(position);
        b->parsePossibleMoves(*p, position);

    } else if (piece == 'B') {
        p = new Bishop(piece, 'w');
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'Q') {
        p = new Queen(piece, 'w');
        p->getAllPossibleMoves(std::make_pair(x, y));

    } else if (piece == 'K') {
        bool inStartingPosition = true;

        if (x != 'e' || y != 1) {
            inStartingPosition = false;
        }

        p = new King(piece, 'w', inStartingPosition);
        p->getAllPossibleMoves(std::make_pair(x, y));

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

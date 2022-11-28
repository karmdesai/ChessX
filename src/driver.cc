#include <algorithm>
#include <iostream>

#include "board/board.h"
#include "computer/abstractPlayer.h"
#include "computer/computer1.h"
#include "computer/computer2.h"
#include "computer/computer3.h"
#include "computer/computer4.h"
#include "computer/human.h"
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
      if (setupCommand == "+") {
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
      } else if (setupCommand == "done") {
        // Check if the Board has a valid setup.
        std::vector<std::string> errors;

        int whiteKings = 0;
        int blackKings = 0;

        std::pair<char, int> position;

        for (int x = 0; x < 8; x++) {
          for (int y = 0; y < 8; y++) {
            position = std::make_pair(b->convertNumToAlpha(x), y + 1);

            Piece *currentPiece = b->getPieceAtPosition(position);

            // no pawns allowed on first/last row.
            if ((currentPiece->getName() == 'p' ||
                 currentPiece->getName() == 'P') &&
                (y == 0 || y == 7)) {
              errors.push_back(
                  "You cannot put a pawn on the first or last "
                  "rank. Please remove the pawn.");
            } else if (currentPiece->getName() == 'K') {
              // if the King is in check, invalid setup.
              if (b->inCheck(*(currentPiece))) {
                errors.push_back("The white King should not be in check.");
              }

              whiteKings += 1;
            } else if (currentPiece->getName() == 'k') {
              // if the King is in check, invalid setup.
              if (b->inCheck(*(currentPiece))) {
                errors.push_back("The black King should not be in check.");
              }

              blackKings += 1;
            }
          }
        }

        if ((blackKings != 1)) {
          errors.push_back("There should be exactly 1 black King.");
        }

        if ((whiteKings != 1)) {
          errors.push_back("There should be exactly 1 white King.");
        }

        if (errors.size() == 0) {
          std::cout << "Successfully configured board." << std::endl;
          break;
        } else {
          std::cout << "You must fix the following errors before "
                       "exiting setup mode: "
                    << std::endl;

          for (auto error : errors) {
            std::cout << "  - " << error << std::endl;
          }

          std::cout << std::endl;
        }
      }
    }
  } else {
    b->defaultInitialization();
  }

  std::cout << b << std::endl;
  /* End Board Setup */

  std::cout << "Enter a piece followed by current position to get all possible "
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
    b->parsePossibleMoves(*p, position);

  } else if (piece == 'n') {
    p = new Knight(piece, 'b');

    p->getAllPossibleMoves(position);
    b->parsePossibleMoves(*p, position);

  } else if (piece == 'b') {
    p = new Bishop(piece, 'b');

    p->getAllPossibleMoves(position);
    b->parsePossibleMoves(*p, position);

  } else if (piece == 'q') {
    p = new Queen(piece, 'b');

    p->getAllPossibleMoves(position);
    b->parsePossibleMoves(*p, position);

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
    b->parsePossibleMoves(*p, position);

  } else if (piece == 'N') {
    p = new Knight(piece, 'w');

    p->getAllPossibleMoves(position);
    b->parsePossibleMoves(*p, position);

  } else if (piece == 'B') {
    p = new Bishop(piece, 'w');

    p->getAllPossibleMoves(position);
    b->parsePossibleMoves(*p, position);

  } else if (piece == 'Q') {
    p = new Queen(piece, 'w');

    p->getAllPossibleMoves(position);
    b->parsePossibleMoves(*p, position);

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
  }

  /*
  Driver code for testing moving with the computer:
    * Takes input in the form "move <old position> <new position>".
    * For example, "move a2 a4" moves the piece at a2 to a4.
  */

  // create a human player and a level-1 computer player
  AbstractPlayer *human = new Human{'w', b};         // human is white
  AbstractPlayer *opponent = new Computer2{'b', b};  // computer is black

  b->setTurn('w');
  while (true) {
    // print who's turn it is
    if (b->getTurn() == 'w') {
      std::cout << "White's turn: " << std::endl;
    } else {
      std::cout << "Black's turn: " << std::endl;
      auto move = opponent->calculateNextMove();
      // delete b->getPieceAtPosition(newPos);
      Piece *computerChoicePiece = b->getPieceAtPosition(move.first);
      computerChoicePiece->setPieceAsMoved();
      b->setPieceAtPosition(move.second, computerChoicePiece);
      b->setPieceAtPosition(move.first, new NullPiece{'*', '*'});
      b->setTurn(human->getPlayerColor());
    }
    // read in stuff
    std::string ask;
    char oldPosX, oldPosY, newPosX, newPosY;
    std::cin >> ask >> oldPosX >> oldPosY >> newPosX >> newPosY;

    std::pair<char, int> oldPos = std::make_pair(oldPosX, oldPosY - '0');
    std::pair<char, int> newPos = std::make_pair(newPosX, newPosY - '0');

    // if there's no piece there
    Piece *pieceAtPosition = b->getPieceAtPosition(oldPos);
    if (pieceAtPosition->getName() == '*') {
      std::cout << "No piece at that position" << std::endl;
      continue;
    } else {
      pieceAtPosition->getAllPossibleMoves(oldPos);
      b->parsePossibleMoves(*pieceAtPosition, oldPos);
    }

    if (ask == "exit") {
      break;
    }

    // if the piece is not the right color or move is out of bounds
    if (ask != "move" || oldPosX < 'a' || oldPosX > 'h' || oldPosY < '1' ||
        oldPosY > '8' || newPosX < 'a' || newPosX > 'h' || newPosY < '1' ||
        newPosY > '8' || b->getTurn() != pieceAtPosition->getColor()) {
      std::cout << "Invalid command" << std::endl;
      continue;

      // if move isnt in possible moves of the piece
    } else if (std::find(pieceAtPosition->allPossibleMoves.begin(),
                         pieceAtPosition->allPossibleMoves.end(),
                         newPos) == pieceAtPosition->allPossibleMoves.end()) {
      std::cout << "Invalid move" << std::endl;
      continue;
      // move should be fine
    }

    else {
      // if its a human's turn, move the piece
      if (b->getTurn() == human->getPlayerColor()) {
        // delete b->getPieceAtPosition(newPos);
        pieceAtPosition->setPieceAsMoved();
        b->setPieceAtPosition(newPos, pieceAtPosition);
        b->setPieceAtPosition(oldPos, new NullPiece{'*', '*'});
        b->setTurn(opponent->getPlayerColor());
      } else {
        // auto move = opponent->calculateNextMove();
        // // delete b->getPieceAtPosition(newPos);
        // Piece *computerChoicePiece = b->getPieceAtPosition(move.first);
        // computerChoicePiece->setPieceAsMoved();
        // b->setPieceAtPosition(move.second, computerChoicePiece);
        // b->setPieceAtPosition(move.first, new NullPiece{'*', '*'});
        // b->setTurn(human->getPlayerColor());
      }

      // print the board
      std::cout << b << std::endl;
    }
  }

  /*
  Testing whether clone works or not
  */

  // clone board
  Board *b2 = b->clone();

  // print addresses of each piece in the board
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (b->getPieceAtPosition(std::make_pair('a' + j, 8 - i)) ==
          b2->getPieceAtPosition(std::make_pair('a' + j, 8 - i))) {
        std::cout << "SAME PIECE (THIS IS BAD) " << (char)('a' + j) << 8 - i
                  << std::endl;
      }
    }
  }

  if (b == b2) {
    std::cout << "SAME BOARD (THIS IS BAD)" << std::endl;
  } else {
    std::cout << "Different board" << std::endl;
  }
  delete b2;

  // if (p != nullptr) {
  //   for (auto move : p->allPossibleMoves) {
  //     std::cout << move.first << move.second << ", ";
  //   }

  //   std::cout << std::endl;

  //   delete p;
  // }

  delete b;
}

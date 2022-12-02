#include <algorithm>
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

#include "observers/studio.h"
#include "observers/textObserver.h"
#include "observers/GUIObs.h"

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
  Studio *s = new Studio{b};
  TextObs* obs = new TextObs{ s };
  GraphObs* guiobs = new GraphObs{ s };

  /* Start Board Setup */
  if (firstCommand == "setup") {
    s->render(std::make_pair('o', 0), std::make_pair('o', 0));

    std::string setupCommand;

    while (std::cin >> setupCommand) {
      if (setupCommand == "+") {
        char piece;
        char x;
        int y;

        std::cin >> piece >> x >> y;
        std::pair<char, int> position = std::make_pair(x, y);

        Piece* pieceCurrent = b->getPieceAtPosition(position);

        // if there's already a piece, they need to remove it first.
        if (pieceCurrent->getName() != '*') {
          std::cout << "There is already a piece at this position." << std::endl;
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
            // since the piece is a NullPiece, we must delete before placing a new one.
            delete b->getPieceAtPosition(position);
            b->setPieceAtPosition(position, newPiece);
          }

          s->render(std::make_pair('o', 0), std::make_pair('o', 0));
        }
      } else if (setupCommand == "-") {
        char x;
        int y;

        std::cin >> x >> y;
        std::pair<char, int> position = std::make_pair(x, y);

        if (b->getPieceAtPosition(position)->getName() != '*') {
          delete b->getPieceAtPosition(position);
          b->setPieceAtPosition(position, new NullPiece('*', '*'));

          s->render(std::make_pair('o', 0), std::make_pair('o', 0));
        }
      } else if (setupCommand == "=") {
        char nextPlayer;

        std::cin >> nextPlayer;

        b->setTurn(nextPlayer);
      } else if (setupCommand == "done") {
        b->generateCompleteMoves();
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
              errors.push_back("You cannot put a pawn on the first or last "
                               "rank. Please remove the pawn.");
            } else if (currentPiece->getName() == 'K') {
              b->setWhiteKing(currentPiece);
              b->setWhiteKingPosition(position);

              // if the King is in check, invalid setup.
              if (b->inCheck(*(currentPiece), position)) {
                errors.push_back("The white King should not be in check.");
              }

              whiteKings += 1;
            } else if (currentPiece->getName() == 'k') {
              b->setBlackKing(currentPiece);
              b->setBlackKingPosition(position);

              // if the King is in check, invalid setup.
              if (b->inCheck(*(currentPiece), position)) {
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

  std::cout << std::endl;
  std::cout << "Start the Game!" << std::endl;
  s->render(std::make_pair('o', 0), std::make_pair('o', 0));

  /* Start Moves Parser Testing */
  /*
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
    b->parsePossibleMoves(*p, position);

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
    b->parsePossibleMoves(*p, position);

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
  */
  /* End Moves Parser Testing */

  /* Start Game Testing */
  std::string command;
  std::pair<char, int> position;

  while (std::cin >> command) {
    b->generateCompleteMoves();

    if (command == "move") {
      char oldX;
      int oldY;

      char newX;
      int newY;

      std::cin >> oldX >> oldY >> newX >> newY;
      std::pair<char, int> oldPosition = std::make_pair(oldX, oldY);
      std::pair<char, int> newPosition = std::make_pair(newX, newY);

      b->movePiece(oldPosition, newPosition);

      s->render(oldPosition, newPosition);
      
      if (b->inCheck(*(b->getBlackKing()), b->getBlackKingPosition())) {
        std::cout << "The Black King is in check!" << std::endl;
      } else if (b->inCheck(*(b->getWhiteKing()), b->getWhiteKingPosition())) {
        std::cout << "The White King is in check!" << std::endl;
      }
    } else if (command == "movelist") {
      char x;
      int y;

      std::cin >> x >> y;

      for (auto move : b->getPieceAtPosition(std::make_pair(x, y))->allPossibleMoves) {
        std::cout << move.first << move.second << ",";
      }

      std::cout << std::endl;
    } else if (command == "tester") {
      // move white king to d5
      b->movePiece(std::make_pair('e', 2), std::make_pair('e', 4));
      b->movePiece(std::make_pair('e', 1), std::make_pair('e', 2));
      b->movePiece(std::make_pair('e', 2), std::make_pair('e', 3));
      b->movePiece(std::make_pair('e', 3), std::make_pair('d', 4));
      b->movePiece(std::make_pair('d', 4), std::make_pair('d', 5));

      std:: cout << "ORIGINAL: " << std::endl;
      s->render(std::make_pair('e', 2), std::make_pair('e', 4));

      Board *newBoard = b->clone();

      std::cout << "CLONE: " << std::endl;
      std::cout << newBoard << std::endl;

      newBoard->movePiece(std::make_pair('d', 5), std::make_pair('d', 6));

      std:: cout << "ORIGINAL: " << std::endl;
      s->render(std::make_pair('e', 2), std::make_pair('e', 4));

      std::cout << "CLONE: " << std::endl;
      std::cout << newBoard << std::endl;

      if (newBoard->inCheck(*(newBoard->getWhiteKing()), newBoard->getWhiteKingPosition())) {
        std::cout << "The White King is in check!" << std::endl;
      } else if (newBoard->inCheck(*(newBoard->getBlackKing()), newBoard->getBlackKingPosition())) {
        std::cout << "The Black King is in check!" << std::endl;
      } else {
        b->movePiece(std::make_pair('d', 5), std::make_pair('d', 6));
      }

      std:: cout << "ORIGINAL: " << std::endl;
      s->render(std::make_pair('e', 2), std::make_pair('e', 4));

      std::cout << "CLONE: " << std::endl;
      std::cout << newBoard << std::endl;

      delete newBoard;

    } else {
        break;
    }
  }
  /* End Game Testing */
  delete obs;
  delete guiobs;
  delete b;
}

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

int main(int argc, char *argv[]) {
  std::cout << "**** WELCOME TO CHESS ****" << std::endl;

  std::cout << "Please enter the setup command immediately if you would like "
               "to use a custom setup."
            << std::endl;
  std::cout << "You will not be able to use a custom setup later on."
            << std::endl;
  std::cout << std::endl;

  // check for whether user wants default setup or custom setup through
  // commandline flag
  bool customSetup = false;
  if (argc > 1) {
    std::string flag = argv[1];
    if (flag == "-c") {
      customSetup = true;
    }
  }

  // you can only setup on the very first command for now.
  // std::string firstCommand;
  // std::cin >> firstCommand;

  Board *b = new Board();

  /* Start Board Setup */
  if (customSetup) {
    std::cout << b << std::endl;

    std::string setupCommand;

    while (std::cin >> setupCommand) {
      if (setupCommand == "+") {
        char piece;
        char x;
        int y;

        std::cin >> piece >> x >> y;
        std::pair<char, int> position = std::make_pair(x, y);

        Piece *pieceCurrent = b->getPieceAtPosition(position);

        // if there's already a piece, they need to remove it first.
        if (pieceCurrent->getName() != '*') {
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
            // since the piece is a NullPiece, we must delete before placing a
            // new one.
            delete b->getPieceAtPosition(position);
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

        b->setColourTurn(nextPlayer);
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
              errors.push_back(
                  "You cannot put a pawn on the first or last "
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
  std::cout << b << std::endl;

  // we always have two computers in the background, which check for
  // checkmate or stalemate. They don't actually play the game.
  AbstractPlayer *whiteChecker = new Computer3('w', b);
  AbstractPlayer *blackChecker = new Computer3('b', b);

  /* Start Game Testing */
  b->setWhitePlayer(new Computer3('w', b));
  b->setBlackPlayer(new Computer3('b', b));

  std::string command;
  // std::pair<char, int> position;
  b->setColourTurn('w');
  b->setPlayerTurn(b->getWhitePlayer());

  while (!std::cin.eof()) {
    b->generateCompleteMoves();
    // check for checkmate or stalemate on both sides.
    if (whiteChecker->calculateNextMove() ==
        std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1))) {
      std::cout << "Checkmate! Black wins!" << std::endl;
      delete b;
      return 0;
    }

    else if (blackChecker->calculateNextMove() ==
             std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1))) {
      std::cout << "Checkmate! White wins!" << std::endl;
      delete b;
      return 0;
    }

    else if (whiteChecker->calculateNextMove() ==
                 std::make_pair(std::make_pair('a', 0),
                                std::make_pair('a', 0)) ||
             blackChecker->calculateNextMove() ==
                 std::make_pair(std::make_pair('a', 0),
                                std::make_pair('a', 0))) {
      std::cout << "Stalemate! It's a draw!" << std::endl;
      delete b;
      return 0;
    }

    AbstractPlayer *currentPlayer = b->getWhosPlayerTurn();

    if (b->getTurn() == 'w') {
      std::cout << "White's turn: " << std::endl;
    } else {
      std::cout << "Black's turn: " << std::endl;
    }

    std::cin >> command;

    if (command == "move") {
      char oldX;
      int oldY;

      char newX;
      int newY;

      // if its the computer's move, tell it to calculate its move
      if (currentPlayer->isComputer()) {
        auto move = currentPlayer->calculateNextMove();
        // print out the move
        std::cout << "Computer plays: " << move.first.first << move.first.second
                  << " -> " << move.second.first << move.second.second
                  << std::endl;

        bool movedSucessfully = b->movePiece(move.first, move.second);

        // if the move was invalid, retry the move.
        if (!movedSucessfully) {
          std::cout << "Something very bad happened. The computer made an "
                       "invalid move. The program has been terminated."
                    << std::endl;
          break;
        }

        std::cout << b << std::endl;

        if (b->inCheck(*(b->getBlackKing()), b->getBlackKingPosition())) {
          std::cout << "The Black King is in check!" << std::endl;
        } else if (b->inCheck(*(b->getWhiteKing()),
                              b->getWhiteKingPosition())) {
          std::cout << "The White King is in check!" << std::endl;
        }
      } else {
        std::cin >> oldX >> oldY >> newX >> newY;

        /* if any of the values read are out of bounds, retry the move.
        usually we would want this to happen in the movePiece function
        but it fucking seg faults for some reason, so we do it here. */
        if (oldX < 'a' || oldX > 'h' || oldY < 1 || oldY > 8 || newX < 'a' ||
            newX > 'h' || newY < 1 || newY > 8) {
          std::cout << "Invalid move. Please try again." << std::endl;
          continue;
        }

        std::pair<char, int> oldPosition = std::make_pair(oldX, oldY);
        std::pair<char, int> newPosition = std::make_pair(newX, newY);

        bool movedSucessfully = b->movePiece(oldPosition, newPosition);

        // if the move was invalid in any way, retry the move.
        if (!movedSucessfully) {
          std::cout << "Invalid move. Please try again." << std::endl;
          continue;
        }

        std::cout << b << std::endl;

        if (b->inCheck(*(b->getBlackKing()), b->getBlackKingPosition())) {
          std::cout << "The Black King is in check!" << std::endl;
        } else if (b->inCheck(*(b->getWhiteKing()),
                              b->getWhiteKingPosition())) {
          std::cout << "The White King is in check!" << std::endl;
        }
      }

    } else if (command == "exit") {
      break;
    } else {
      // retry the move.
      std::cout << "Invalid command. Please try again." << std::endl;
      continue;
    }

    // set player and turn to the next player
    if (b->getTurn() == 'w') {
      b->setColourTurn('b');
      b->setPlayerTurn(b->getBlackPlayer());
    } else {
      b->setColourTurn('w');
      b->setPlayerTurn(b->getWhitePlayer());
    }
  }
  /* End Game Testing */

  delete b;
}

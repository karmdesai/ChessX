#include <algorithm>
#include <cctype>
#include <cstring>
#include <iostream>
#include <memory>

#include "board/board.h"
#include "computer/abstractPlayer.h"
#include "computer/computer1.h"
#include "computer/computer2.h"
#include "computer/computer3.h"
#include "computer/computer4.h"
#include "computer/human.h"
#include "observers/GUIObs.h"
#include "observers/studio.h"
#include "observers/textObserver.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/nullPiece.h"
#include "pieces/pawn.h"
#include "pieces/piece.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

const int MAX_TRIES = 10;

// struct to keep track of number of games played and the results
struct Stats {
  int numGames;
  int numWinsWhite;
  int numWinsBlack;
  int numDraws;

  void printStats() {
    std::cout << "Final Score:" << std::endl;
    std::cout << "White: " << numWinsWhite << std::endl;
    std::cout << "Black: " << numWinsBlack << std::endl;
    std::cout << "Draws: " << numDraws << std::endl;
  }
};

// struct to hold the result of a particular game
struct Result {
  char winnerColour;
  bool isDraw;
};

// deletes all heap-allocated memory after a game is finished
void cleanup(Board &b, AbstractPlayer &whiteChecker,
             AbstractPlayer &blackChecker, AbstractPlayer &whitePlayer,
             AbstractPlayer &blackPlayer) {
  delete &b;
  delete &whiteChecker;
  delete &blackChecker;
  delete &whitePlayer;
  delete &blackPlayer;
  return;
}

// initializes a custom board setup according to how the user wants it
bool initializeBoard(Board *b, Studio *s) {
  s->render(std::make_pair('o', 0), std::make_pair('o', 0), false);

  std::string setupCommand;

  while (!std::cin.eof()) {
    bool choseWhoGoesFirst = false;
    std::cin >> setupCommand;

    if (setupCommand == "=") {
      std::string color;
      std::cin >> color;

      if (color == "white") {
        b->setColourTurn('w');
      } else if (color == "black") {
        b->setColourTurn('b');
      } else {
        std::cout << "Invalid color. Please enter 'white' or 'black'."
                  << std::endl;
      }
      choseWhoGoesFirst = true;

    } else if (setupCommand == "+") {
      char piece;
      char x;
      int y;

      std::cin >> piece >> x >> y;
      std::pair<char, int> position = std::make_pair(x, y);

      Piece *pieceCurrent = b->getPieceAtPosition(position);

      // if there's already a piece, they need to remove it first.
      if (pieceCurrent->getName() != '*') {
        std::cout << "There is already a piece at this position." << std::endl;
        std::cout << "Remove the piece first or select a different "
                     "position."
                  << std::endl;
      } else {
        // Piece *newPiece = b->createPiece(piece);
        // check if piece is a valid piece
        if (tolower(piece) == 'p' || tolower(piece) == 'r' ||
            tolower(piece) == 'n' || tolower(piece) == 'b' ||
            tolower(piece) == 'q' || tolower(piece) == 'k') {
          Piece *newPiece = b->createPiece(piece);
          // delete what is currently at the position
          delete pieceCurrent;
          b->setPieceAtPosition(position, newPiece);
          s->render(position, position, false);
        } else {
          std::cout << "Invalid piece. Please enter a valid piece."
                    << std::endl;
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

        s->render(position, position, false);
      }
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
        // b->generateCompleteMoves();
        if (!choseWhoGoesFirst) {
          // set to white by default.
          b->setColourTurn('w');
        }
        return true;
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
  return false;  // this means the user ctrl-d'd
}

// sets up the players for a game (human or computer)
bool setupPlayers(Board *b) {
  std::string command, whitePlayer, blackPlayer;

  while (true) {
    std::cout << "To start a new game, enter 'game white-player "
                 "black-player', where white-player and black-player are "
                 "either 'human' or one of 'computer[1-4]'."
              << std::endl;
    std::cin >> command >> whitePlayer >> blackPlayer;

    if (std::cin.eof()) {
      return false;
    }

    if (command != "game") {
      std::cout << "Invalid command. Please follow the format "
                   "'game white-player black-player'."
                << std::endl;
      continue;
    } else if (whitePlayer != "human" && whitePlayer != "computer1" &&
               whitePlayer != "computer2" && whitePlayer != "computer3" &&
               whitePlayer != "computer4") {
      std::cout << "Invalid white player. Please follow the format "
                   "'game white-player black-player'."
                << std::endl;
      continue;
    } else if (blackPlayer != "human" && blackPlayer != "computer1" &&
               blackPlayer != "computer2" && blackPlayer != "computer3" &&
               blackPlayer != "computer4") {
      std::cout << "Invalid black player. Please follow the format "
                   "'game white-player black-player'."
                << std::endl;
      continue;
    } else {
      break;
    }
  }

  // create new players according to what the user specified.
  if (whitePlayer == "human") {
    b->setWhitePlayer(new Human('w', b));
  } else if (whitePlayer == "computer1") {
    b->setWhitePlayer(new Computer1('w', b));
  } else if (whitePlayer == "computer2") {
    b->setWhitePlayer(new Computer2('w', b));
  } else if (whitePlayer == "computer3") {
    b->setWhitePlayer(new Computer3('w', b));
  } else if (whitePlayer == "computer4") {
    b->setWhitePlayer(new Computer4('w', b));
  }

  if (blackPlayer == "human") {
    b->setBlackPlayer(new Human('b', b));
  } else if (blackPlayer == "computer1") {
    b->setBlackPlayer(new Computer1('b', b));
  } else if (blackPlayer == "computer2") {
    b->setBlackPlayer(new Computer2('b', b));
  } else if (blackPlayer == "computer3") {
    b->setBlackPlayer(new Computer3('b', b));
  } else if (blackPlayer == "computer4") {
    b->setBlackPlayer(new Computer4('b', b));
  }
  return true;
}

// function to play a game of chess
Result playGame(Board *b, Studio *s, bool defaultInit) {
  std::cout << std::endl;
  std::cout << "Start the Game!" << std::endl;
  if (defaultInit) {
    s->render(std::make_pair('o', 0), std::make_pair('o', 0), false);
  }

  // we always have two computers in the background, which check for
  // checkmate or stalemate. They don't actually play the game.
  AbstractPlayer *whiteChecker = new Computer1('w', b);
  AbstractPlayer *blackChecker = new Computer1('b', b);

  std::string command;
  // set next player's turn
  char nextTurn = b->getColourTurn();
  if (nextTurn == 'w') {
    b->setPlayerTurn(b->getWhitePlayer());
  } else {
    b->setPlayerTurn(b->getBlackPlayer());
  }

  while (!std::cin.eof()) {
    b->generateCompleteMoves();

    // check for insufficient material
    if (b->isInsufficientMaterial()) {
      std::cout << "Insufficient material. Game is a draw." << std::endl;
      cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
              *b->getBlackPlayer());
      return {'w', true};
    }

    // check for checkmate or stalemate on both sides.
    if (whiteChecker->calculateNextMove() ==
        std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1))) {
      std::cout << "Checkmate! Black wins!" << std::endl;
      cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
              *b->getBlackPlayer());
      return {'b', false};
    }

    else if (blackChecker->calculateNextMove() ==
             std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1))) {
      std::cout << "Checkmate! White wins!" << std::endl;
      cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
              *b->getBlackPlayer());
      return {'w', false};
    }

    // if it's black turn and there are no moves, it's stalemate.
    else if (b->getColourTurn() == 'b' &&
             blackChecker->calculateNextMove() ==
                 std::make_pair(std::make_pair('a', 0),
                                std::make_pair('a', 0))) {
      std::cout << "Stalemate! It's a draw!" << std::endl;
      cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
              *b->getBlackPlayer());
      return {' ', true};
    }

    // if it's white turn and there are no moves, it's stalemate.
    else if (b->getColourTurn() == 'w' &&
             whiteChecker->calculateNextMove() ==
                 std::make_pair(std::make_pair('a', 0),
                                std::make_pair('a', 0))) {
      std::cout << "Stalemate! It's a draw!" << std::endl;
      cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
              *b->getBlackPlayer());
      return {' ', true};
    }

    AbstractPlayer *currentPlayer = b->getWhosPlayerTurn();

    if (b->getColourTurn() == 'w') {
      std::cout << "White's turn: " << std::endl;
    } else {
      std::cout << "Black's turn: " << std::endl;
    }

    std::cin >> command;

    if (command == "move") {
      char oldX, newX;
      int oldY, newY;

      // if its the computer's move, tell it to calculate its move
      if (currentPlayer->isComputer()) {
        auto move = currentPlayer->calculateNextMove();
        // print out the move
        std::cout << "Computer plays " << move.first.first << move.first.second
                  << " -> " << move.second.first << move.second.second
                  << std::endl;

        bool movedSuccessfully;

        // if piece is white and being promoted, auto promote to queen
        if (b->getPieceAtPosition(move.first)->getColor() == 'w' &&
            move.second.second == 8) {
          // if piece is pawn, promote to queen
          if (b->getPieceAtPosition(move.first)->getName() == 'P') {
            movedSuccessfully = b->movePieceBase(move.first, move.second, 'Q');
          } else {
            movedSuccessfully = b->movePiece(move.first, move.second);
          }
        }

        // if the piece is black and being promoted, auto promote to queen
        else if (b->getPieceAtPosition(move.first)->getColor() == 'b' &&
                 move.second.second == 1) {
          // if piece is pawn, promote to queen
          if (b->getPieceAtPosition(move.first)->getName() == 'p') {
            movedSuccessfully = b->movePieceBase(move.first, move.second, 'q');
          } else {
            movedSuccessfully = b->movePiece(move.first, move.second);
          }
        }

        else {
          movedSuccessfully = b->movePiece(move.first, move.second);
        }
        int count = 0;

        /* So there's this weird bug where the computer might try to move
        a piece that is pinned to its own king. Here I try to force it to
        choose a new move, and give it 10 tries. If it still fails, then I
        just assume the computer has lost its mind and force it to resign.
      */
        while (count < MAX_TRIES && !movedSuccessfully) {
          // tell the computer to calculate its move again.
          auto newMove = currentPlayer->calculateNextMove();
          if (newMove == move) {
            count++;
          } else {
            movedSuccessfully = b->movePiece(newMove.first, newMove.second);
          }
          break;
        }

        if (count == MAX_TRIES) {
          std::cout << "Computer is stuck. Computer resigns." << std::endl;
          // increment the score of the other player.
          if (currentPlayer->getPlayerColor() == 'w') {
            cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
                    *b->getBlackPlayer());
            return {'b', false};
          } else {
            cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
                    *b->getBlackPlayer());
            return {'w', false};
          }
        }

        if (b->getEnPassantMade()) {
          s->render(std::make_pair(move.first.first, move.first.second),
                    std::make_pair(move.second.first, move.second.second),
                    true);
          b->setEnPassantFalse();
        } else {
          s->render(std::make_pair(move.first.first, move.first.second),
                    std::make_pair(move.second.first, move.second.second),
                    false);
        }
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
          std::cout << "Move is out of bounds. Try again!" << std::endl;
          continue;
        }

        std::pair<char, int> oldPosition = std::make_pair(oldX, oldY);
        std::pair<char, int> newPosition = std::make_pair(newX, newY);

        // check if the player is trying to move a piece that isn't theirs.
        if (b->getPieceAtPosition(oldPosition)->getColor() !=
            b->getColourTurn()) {
          std::cout << "You can't move that piece, it's not yours!"
                    << std::endl;
          continue;
        }

        bool enPassant = false;

        std::pair<char, int> startPos = std::make_pair(oldX, oldY);
        std::pair<char, int> endPos = std::make_pair(newX, newY);

        if (b->getPieceAtPosition(startPos)->getName() == 'P' &&
            endPos.first != startPos.first &&
            b->getPieceAtPosition(
                 std::make_pair(endPos.first, endPos.second - 1))
                    ->getName() == 'p') {
          enPassant = true;
        } else if (b->getPieceAtPosition(startPos)->getName() == 'p' &&
                   endPos.first != startPos.first &&
                   b->getPieceAtPosition(
                        std::make_pair(endPos.first, endPos.second + 1))
                           ->getName() == 'P') {
          enPassant = true;
        }
        bool movedSucessfully;

        if (newY == 8 && b->getColourTurn() == 'w' &&
            b->getPieceAtPosition(std::make_pair(oldX, oldY))->getName() ==
                'P') {
          char promote;
          std::cin >> promote;

          while (!isupper(promote) || promote == 'K' || promote == 'P') {
            std::cout << "You can't promote to this piece!" << std::endl;
            std::cin >> promote;
          }
          movedSucessfully =
              b->movePieceBase(oldPosition, newPosition, promote);
        } else if (newY == 1 && b->getColourTurn() == 'b' && b->getPieceAtPosition(
            std::make_pair(oldX, oldY))->getName() == 'p') {
          char promote;
          std::cin >> promote;

          while (!islower(promote) || promote == 'k' || promote == 'p') {
            std::cout << "You can't promote to this piece!" << std::endl;
            std::cin >> promote;
          }
          movedSucessfully =
              b->movePieceBase(oldPosition, newPosition, promote);
        } else {
          movedSucessfully = b->movePiece(oldPosition, newPosition);
        }

        // if the move was invalid in any way, retry the move.
        if (!movedSucessfully) {
          std::cout << "Invalid move. Please try again." << std::endl;
          continue;
        }
        s->render(std::make_pair(oldX, oldY), std::make_pair(newX, newY),
                  enPassant);

        if (b->inCheck(*(b->getBlackKing()), b->getBlackKingPosition())) {
          std::cout << "The Black King is in check!" << std::endl;
        } else if (b->inCheck(*(b->getWhiteKing()),
                              b->getWhiteKingPosition())) {
          std::cout << "The White King is in check!" << std::endl;
        }
      }
    } else if (command == "resign") {
      // increment the score of the color that won.
      if (b->getColourTurn() == 'w') {
        std::cout << "White resigns, Black wins!" << std::endl;
        cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
                *b->getBlackPlayer());
        return {'b', false};
      } else {
        std::cout << "Black resigns, White wins!" << std::endl;
        cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
                *b->getBlackPlayer());
        return {'w', false};
      }
    } else {
      // retry the move.
      std::cout << "Invalid command. Please try again." << std::endl;
      continue;
    }

    // set player and turn to the next player
    if (b->getColourTurn() == 'w') {
      b->setColourTurn('b');
      b->setPlayerTurn(b->getBlackPlayer());
    } else {
      b->setColourTurn('w');
      b->setPlayerTurn(b->getWhitePlayer());
    }
  }
  cleanup(*b, *whiteChecker, *blackChecker, *b->getWhitePlayer(),
          *b->getBlackPlayer());
  // something bad happened, return draw
  return {'w', true};
}

int main() {
  std::cout << "**** WELCOME TO CHESS ****" << std::endl;

  // you can only setup in the beginning
  std::string firstCommand;

  Stats stats = {0, 0, 0, 0};
  while (true) {
    bool defaultInit = true;
    Board *b = new Board();
    Studio s{b};
    TextObs *obs = new TextObs{&s};
    GraphObs *guiobs = new GraphObs{&s};
    std::cout << "Please enter 'setup' immediately if you would like "
                 "to use a custom setup. Otherwise, enter 'done'."
              << std::endl
              << std::endl;
    std::cout << "You will not be able to choose a custom setup until another "
                 "game starts."
              << std::endl
              << std::endl;
    std::cout
        << "If you would like to exit the game, enter 'exit'. After this, "
           "you may only exit after a game has finished."
        << std::endl;

    std::cout << std::endl;

    std::cin >> firstCommand;

    if (std::cin.eof()) {
      delete obs;
      delete guiobs;
      delete b;
      break;
    }

    if (firstCommand == "exit") {
      delete obs;
      delete guiobs;
      delete b;
      break;
    }
    if (firstCommand == "setup") {
      bool success = initializeBoard(b, &s);
      if (!success) {
        delete obs;
        delete guiobs;
        delete b;
        return 0;
      }
      defaultInit = false;
    } else {
      b->defaultInitialization();
    }

    bool success = setupPlayers(b);
    if (!success) {
      delete obs;
      delete guiobs;
      delete b;
      return 0;
    }

    // start the game.
    Result result = playGame(b, &s, defaultInit);

    if (result.isDraw) {
      ++stats.numDraws;
    } else {
      if (result.winnerColour == 'w') {
        ++stats.numWinsWhite;
      } else if (result.winnerColour == 'b') {
        ++stats.numWinsBlack;
      }
    }
    delete obs;
    delete guiobs;
  }
  stats.printStats();
  return 0;
}

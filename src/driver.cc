#include <algorithm>
#include <iostream>
#include <memory>

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

struct Result {
  char winnerColour;
  bool isDraw;
};

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

void initializeBoard(Board *b) {
  std::cout << b << std::endl;

  std::string setupCommand;

  while (std::cin >> setupCommand) {
    bool choseWhoGoesFirst = false;

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
      if (!choseWhoGoesFirst) {
        // set to white by default.
        b->setColourTurn('w');
      }
    }
  }
}

void setupPlayers(Board *b) {
  std::string command, whitePlayer, blackPlayer;

  while (true) {
    std::cout << "To start a new game, enter 'game white-player "
                 "black-player', where white-player and black-player are "
                 "either 'human' or one of 'computer[1-4]'."
              << std::endl;
    std::cin >> command >> whitePlayer >> blackPlayer;

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
}

// function to play a game of chess
Result playGame(Board *b) {
  std::cout << std::endl;
  std::cout << "Start the Game!" << std::endl;
  std::cout << b << std::endl;

  // we always have two computers in the background, which check for
  // checkmate or stalemate. They don't actually play the game.
  AbstractPlayer *whiteChecker = new Computer1('w', b);
  AbstractPlayer *blackChecker = new Computer1('b', b);

  /* Start Game Testing */
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
      char oldX;
      int oldY;

      char newX;
      int newY;

      // if its the computer's move, tell it to calculate its move
      if (currentPlayer->isComputer()) {
        // std::cout << "GOT HEREE??" << std::endl;
        auto move = currentPlayer->calculateNextMove();
        // std::cout << "GOT HEREE?2?" << std::endl;
        // print out the move
        std::cout << "move " << move.first.first << move.first.second << " "
                  << move.second.first << move.second.second << std::endl;

        bool movedSucessfully = b->movePiece(move.first, move.second);
        int count = 0;

        /* So there's this weird bug where the computer might try to move
        a piece that is pinned to its own king. Here I try to force it to
        choose a new move, and give it 10 tries. If it still fails, then I
        just assume the computer has lost its mind and force it to resign.
      */
        while (count < MAX_TRIES && !movedSucessfully) {
          // tell the computer to calculate its move again.
          auto newMove = currentPlayer->calculateNextMove();
          if (newMove == move) {
            count++;
          } else {
            movedSucessfully = b->movePiece(newMove.first, newMove.second);
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
  // something bad happened, return draw
  return {'w', true};
}

int main() {
  std::cout << "**** WELCOME TO CHESS ****" << std::endl;

  // you can only setup on the very first command for now.
  std::string firstCommand;

  Stats stats = {0, 0, 0, 0};
  while (!std::cin.eof()) {
    Board *b = new Board();
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

    /* Start Board Setup */
    std::cin >> firstCommand;
    if (firstCommand == "exit") {
      delete b;
      break;
    }
    if (firstCommand == "setup") {
      initializeBoard(b);
    } else {
      b->defaultInitialization();
    }
    /* End Board Setup */

    setupPlayers(b);

    // start the game.
    Result result = playGame(b);

    if (result.isDraw) {
      ++stats.numDraws;
    } else {
      if (result.winnerColour == 'w') {
        ++stats.numWinsWhite;
      } else if (result.winnerColour == 'b') {
        ++stats.numWinsBlack;
      }
    }
  }
  stats.printStats();
}

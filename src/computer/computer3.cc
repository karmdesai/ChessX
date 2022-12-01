#include "computer3.h"

#include <iostream>
#include <random>

#include "../board/board.h"
#include "../pieces/piece.h"

// the constructor for Computer3
Computer3::Computer3(char playerColor, Board *board)
    : AbstractPlayer{playerColor, board, true} {}

// calculate the next move, returns a pair of moves
std::pair<std::pair<char, int>, std::pair<char, int>>
Computer3::calculateNextMove() {
  /* This version of the computer prefers avoiding capture first, capturing
  moves and then checking the opponent. If there are multiple possible moves
  that fix the criteria, then it will choose one at random. If there are no
  moves that fit this criteria at all, computer chooses a random move. */

  /* Steps:
    1. Get all possible moves
    2. Get moves from all possible moves that avoid capture if possible
    3. Follow Computer2's logic if there are no moves that avoid capture
  */

  // get all possible moves first
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>> allMoves;

  // loop through the board to get all the possible moves
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      std::cout << "i: " << i << " j: " << j << std::endl;
      // get the piece at the current position
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = board->getPieceAtPosition(currentPos);

      // if the piece is not nullPiece and it is our color
      if (p->getName() != '*' && p->getColor() == this->playerColor) {
        // get all possible moves for the piece
        p->getAllPossibleMoves(std::make_pair(char(i + 'a'), j + 1));
        board->parsePossibleMoves(*p, currentPos);

        // add moves to the list
        for (auto move : p->allPossibleMoves) {
          auto theMove = std::make_pair(currentPos, move);
          allMoves.push_back(theMove);
        }
      }
    }
  }

  std::cout << "got past the loop" << std::endl;

  // if the king is in check, we can only make moves that get the king out of
  // check

  std::pair<char, int> kingPos;
  Piece *king;
  // get the king's position
  if (this->playerColor == 'w') {
    kingPos = board->getWhiteKingPosition();
    king = board->getWhiteKing();
  } else {
    kingPos = board->getBlackKingPosition();
    king = board->getBlackKing();
  }

  // if the king is in check
  if (board->inCheck(*king, kingPos)) {
    // list of moves that get the king out of check
    std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
        movesThatGetKingOutOfCheck;

    // loop through all the moves
    for (auto move : allMoves) {
      // make the move on a copy of the board
      Board *boardCopy = board->clone();
      bool success = boardCopy->movePiece(move.first, move.second);
      if (success) {
        movesThatGetKingOutOfCheck.push_back(move);
      }

      // delete the copy of the board
      delete boardCopy;
    }

    // if there are moves that get the king out of check, return one of them
    if (movesThatGetKingOutOfCheck.size() > 0) {
      int randomIndex = randomNumber(0, movesThatGetKingOutOfCheck.size() - 1);
      std::cout << "computer plays: "
                << movesThatGetKingOutOfCheck[randomIndex].first.first
                << movesThatGetKingOutOfCheck[randomIndex].first.second << " "
                << movesThatGetKingOutOfCheck[randomIndex].second.first
                << movesThatGetKingOutOfCheck[randomIndex].second.second
                << std::endl;
      return movesThatGetKingOutOfCheck[randomIndex];
    }
  }
  std::cout << "got past the king check" << std::endl;

  // remove moves from allMoves that put the king in check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatDoNotPutOurKingInCheck;

  // loop through all the moves
  for (auto move : allMoves) {
    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    bool success = boardCopy->movePiece(move.first, move.second);
    std::cout << "was able to make the move: " << success << std::endl;
    std::cout << "move from: " << move.first.first << move.first.second
              << move.second.first << move.second.second << std::endl;

    // if the king is not in check, add the move to the list
    if (success && !boardCopy->inCheck(*king, kingPos)) {
      std::cout << "got inside the check for king2" << std::endl;
      movesThatDoNotPutOurKingInCheck.push_back(move);
      std::cout << "got after push back" << std::endl;
    }

    // delete the copy of the board
    delete boardCopy;
    std::cout << "deleted" << std::endl;
  }

  std::cout << "got past the king check 2" << std::endl;

  // if the king is not in check, we should try to make the best move possible

  // if one of our pieces is under attack, we should try to move it out of
  // attack, to avoid capture
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatAvoidCapture;

  // for each piece of our color, check if it is under attack. If it is, find
  // all the moves so that it is not under attack anymore
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // get the piece at the current position
      std::pair<char, int> currentPos = std::make_pair(char('a' + i), j + 1);
      Piece *p = board->getPieceAtPosition(currentPos);

      // if the piece is not nullPiece and it is our color
      if (p->getName() != '*' && p->getColor() == this->playerColor) {
        // get all possible moves for the piece
        p->getAllPossibleMoves(std::make_pair(char(i + 'a'), j + 1));
        board->parsePossibleMoves(*p, currentPos);

        // if the piece is under attack
        if (board->isPieceCapturable(p, currentPos)) {
          // loop through all the moves
          for (auto move : p->allPossibleMoves) {
            // make the move on a copy of the board
            Board *boardCopy = board->clone();
            bool success = boardCopy->movePiece(currentPos, move);

            // if the piece is not under attack anymore, add the move to the
            // list
            if (success && !boardCopy->isPieceCapturable(p, move)) {
              movesThatAvoidCapture.push_back(std::make_pair(currentPos, move));
            }

            // delete the copy of the board
            delete boardCopy;
          }
        }
      }
    }
  }

  std::cout << "got past the capture check" << std::endl;

  // // loop through all the moves
  // for (auto move : movesThatDoNotPutOurKingInCheck) {
  //   // make the move on a copy of the board
  //   Board *boardCopy = board->clone();
  //   bool success = boardCopy->movePiece(move.first, move.second);

  //   // if the move was successful, check if piece is capturable
  //   if (success) {
  //     // get the piece that was moved
  //     Piece *p = boardCopy->getPieceAtPosition(move.second);

  //     // if the piece is not capturable, add it to the list of moves that
  //     avoid
  //     // capture
  //     if (!boardCopy->isPieceCapturable(p, move.second)) {
  //       movesThatAvoidCapture.push_back(move);
  //     }
  //   }

  //   // delete the copy of the board
  //   delete boardCopy;
  // }

  // **********************************************************************
  // FROM HERE, IT IS COMPUTER2'S LOGIC
  // **********************************************************************

  // get all moves from movesThatDoNotPutOurKingInCheck that capture a piece
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatCapture;

  // loop through all the moves
  for (auto move : movesThatDoNotPutOurKingInCheck) {
    // get the piece at the destination
    Piece *p = board->getPieceAtPosition(move.second);

    // if the piece is not nullPiece, add the move to the list
    if (p->getName() != '*' && p->getColor() != this->playerColor) {
      movesThatCapture.push_back(move);
    }
  }

  std::cout << "got past the capture check 2" << std::endl;

  // get all moves that put the opponent in check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatPutOpponentInCheck;

  // loop through all the moves
  for (auto move : movesThatDoNotPutOurKingInCheck) {
    // make the move on a copy of the board
    Board *boardCopy = board->clone();
    boardCopy->movePieceBase(move.first, move.second);

    Piece *king;
    std::pair<char, int> kingPos;

    // get the opponent's king's position
    if (this->playerColor == 'w') {
      kingPos = boardCopy->getBlackKingPosition();
      king = boardCopy->getBlackKing();
    } else {
      kingPos = boardCopy->getWhiteKingPosition();
      king = boardCopy->getWhiteKing();
    }

    // if the king is in check, add the move to the list
    if (boardCopy->inCheck(*king, kingPos)) {
      movesThatPutOpponentInCheck.push_back(move);
    }

    // delete the copy of the board
    delete boardCopy;
  }

  std::cout << "got past the check check" << std::endl;

  // get set intersection of movesThatAvoidCapture, movesThatCapture, and
  // movesThatPutOpponentInCheck
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck;

  // loop through all the moves
  for (auto move : movesThatAvoidCapture) {
    // if the move is in movesThatCapture and movesThatPutOpponentInCheck,
    // add it to the list
    if (std::find(movesThatCapture.begin(), movesThatCapture.end(), move) !=
            movesThatCapture.end() &&
        std::find(movesThatPutOpponentInCheck.begin(),
                  movesThatPutOpponentInCheck.end(),
                  move) != movesThatPutOpponentInCheck.end()) {
      movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck.push_back(move);
    }
  }
  std::cout << "------------------" << std::endl;
  std::cout << "all three intersection" << std::endl;
  for (auto move : movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck) {
    std::cout << move.first.first << move.first.second << " "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------------" << std::endl;

  if (movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck.size() > 0) {
    int randomIndex = randomNumber(
        0, movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck.size() - 1);
    std::cout
        << "computer plays: "
        << movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck[randomIndex]
               .first.first
        << movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck[randomIndex]
               .first.second
        << " "
        << movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck[randomIndex]
               .second.first
        << movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck[randomIndex]
               .second.second
        << std::endl;
    return movesThatAvoidCaptureAndCaptureAndPutOpponentInCheck[randomIndex];
  }

  std::cout << "------------------" << std::endl;
  std::cout << "moves that avoid capture" << std::endl;
  for (auto move : movesThatAvoidCapture) {
    std::cout << move.first.first << move.first.second << " "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------------" << std::endl;

  // moves that avoid capture and capture
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatAvoidCaptureAndCapture;

  // loop through all the moves
  for (auto move : movesThatAvoidCapture) {
    // if the move is in movesThatCapture, add it to the list
    if (std::find(movesThatCapture.begin(), movesThatCapture.end(), move) !=
        movesThatCapture.end()) {
      movesThatAvoidCaptureAndCapture.push_back(move);
    }
  }
  std::cout << "------------------" << std::endl;
  std::cout << "avoid capture and capture" << std::endl;
  for (auto move : movesThatAvoidCaptureAndCapture) {
    std::cout << move.first.first << move.first.second << " "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------------" << std::endl;

  // if there are moves that avoid capture and capture, return one of them
  if (movesThatAvoidCaptureAndCapture.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatAvoidCaptureAndCapture.size() - 1);
    std::cout << "computer plays: "
              << movesThatAvoidCaptureAndCapture[randomIndex].first.first
              << movesThatAvoidCaptureAndCapture[randomIndex].first.second
              << " "
              << movesThatAvoidCaptureAndCapture[randomIndex].second.first
              << movesThatAvoidCaptureAndCapture[randomIndex].second.second
              << std::endl;
    return movesThatAvoidCaptureAndCapture[randomIndex];
  }

  // moves that avoid capture and put opponent in check
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatAvoidCaptureAndPutOpponentInCheck;

  // loop through all the moves
  for (auto move : movesThatAvoidCapture) {
    // if the move is in movesThatPutOpponentInCheck, add it to the list
    if (std::find(movesThatPutOpponentInCheck.begin(),
                  movesThatPutOpponentInCheck.end(),
                  move) != movesThatPutOpponentInCheck.end()) {
      movesThatAvoidCaptureAndPutOpponentInCheck.push_back(move);
    }
  }

  std::cout << "------------------" << std::endl;
  std::cout << "avoid capture and put opponent in check" << std::endl;
  for (auto move : movesThatAvoidCaptureAndPutOpponentInCheck) {
    std::cout << move.first.first << move.first.second << " "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------------" << std::endl;

  // if there are moves that avoid capture and put opponent in check, return one
  // of them
  if (movesThatAvoidCaptureAndPutOpponentInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatAvoidCaptureAndPutOpponentInCheck.size() - 1);
    std::cout
        << "computer plays: "
        << movesThatAvoidCaptureAndPutOpponentInCheck[randomIndex].first.first
        << movesThatAvoidCaptureAndPutOpponentInCheck[randomIndex].first.second
        << " "
        << movesThatAvoidCaptureAndPutOpponentInCheck[randomIndex].second.first
        << movesThatAvoidCaptureAndPutOpponentInCheck[randomIndex].second.second
        << std::endl;
    return movesThatAvoidCaptureAndPutOpponentInCheck[randomIndex];
  }

  // just choose a random move from movesThatAvoidCapture
  if (movesThatAvoidCapture.size() > 0) {
    int randomIndex = randomNumber(0, movesThatAvoidCapture.size() - 1);
    std::cout << "computer plays: "
              << movesThatAvoidCapture[randomIndex].first.first
              << movesThatAvoidCapture[randomIndex].first.second << " "
              << movesThatAvoidCapture[randomIndex].second.first
              << movesThatAvoidCapture[randomIndex].second.second << std::endl;
    return movesThatAvoidCapture[randomIndex];
  }

  // if both above lists are empty, just choose a random move as if
  // we were Computer2 (as Computer2's moves are a subset of Computer3's moves)

  // get intersection of movesThatCapture and movesThatPutOpponentInCheck
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatCaptureAndPutOpponentInCheck;

  // using std::set_intersection
  // source:
  // https://stackoverflow.com/questions/19483663/vector-intersection-in-c
  std::set_intersection(
      movesThatCapture.begin(), movesThatCapture.end(),
      movesThatPutOpponentInCheck.begin(), movesThatPutOpponentInCheck.end(),
      std::back_inserter(movesThatCaptureAndPutOpponentInCheck));

  std::cout << "------------------" << std::endl;
  std::cout << "capture and check intersection" << std::endl;
  for (auto move : movesThatCaptureAndPutOpponentInCheck) {
    std::cout << move.first.first << move.first.second << " "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------------" << std::endl;

  if (movesThatCaptureAndPutOpponentInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatCaptureAndPutOpponentInCheck.size() - 1);
    std::cout
        << "computer plays: "
        << movesThatCaptureAndPutOpponentInCheck[randomIndex].first.first
        << movesThatCaptureAndPutOpponentInCheck[randomIndex].first.second
        << " "
        << movesThatCaptureAndPutOpponentInCheck[randomIndex].second.first
        << movesThatCaptureAndPutOpponentInCheck[randomIndex].second.second
        << std::endl;
    return movesThatCaptureAndPutOpponentInCheck[randomIndex];
  }

  // if there's no intersecting moves, we just choose a random move from the
  // union of movesThatCapture and movesThatPutOpponentInCheck

  // get union of movesThatCapture and movesThatPutOpponentInCheck
  std::vector<std::pair<std::pair<char, int>, std::pair<char, int>>>
      movesThatCaptureOrPutOpponentInCheck;

  // using std::set_union
  std::set_union(movesThatCapture.begin(), movesThatCapture.end(),
                 movesThatPutOpponentInCheck.begin(),
                 movesThatPutOpponentInCheck.end(),
                 std::back_inserter(movesThatCaptureOrPutOpponentInCheck));

  std::cout << "------------------" << std::endl;
  std::cout << "capture or check union" << std::endl;
  for (auto move : movesThatCaptureOrPutOpponentInCheck) {
    std::cout << move.first.first << move.first.second << " "
              << move.second.first << move.second.second << std::endl;
  }
  std::cout << "------------------" << std::endl;

  if (movesThatCaptureOrPutOpponentInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatCaptureOrPutOpponentInCheck.size() - 1);
    std::cout << "computer plays: "
              << movesThatCaptureOrPutOpponentInCheck[randomIndex].first.first
              << movesThatCaptureOrPutOpponentInCheck[randomIndex].first.second
              << " "
              << movesThatCaptureOrPutOpponentInCheck[randomIndex].second.first
              << movesThatCaptureOrPutOpponentInCheck[randomIndex].second.second
              << std::endl;
    return movesThatCaptureOrPutOpponentInCheck[randomIndex];
  }

  // if there are no moves that capture or put the opponent in check, we just
  // choose a random move from movesThatDoNotPutOurKingInCheck

  if (movesThatDoNotPutOurKingInCheck.size() > 0) {
    int randomIndex =
        randomNumber(0, movesThatDoNotPutOurKingInCheck.size() - 1);
    std::cout << "computer plays: "
              << movesThatDoNotPutOurKingInCheck[randomIndex].first.first
              << movesThatDoNotPutOurKingInCheck[randomIndex].first.second
              << " "
              << movesThatDoNotPutOurKingInCheck[randomIndex].second.first
              << movesThatDoNotPutOurKingInCheck[randomIndex].second.second
              << std::endl;
    return movesThatDoNotPutOurKingInCheck[randomIndex];
  }

  // if there are no moves AT ALL its a stalemate, as the king isn't in check
  // (otherwise we would have returned a move that gets the king out of check
  // or would have realized that we've been checkmated)
  return std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));
}

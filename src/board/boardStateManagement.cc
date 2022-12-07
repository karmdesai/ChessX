#include "board.h"

#include "../pieces/nullPiece.h"

// returns a boolean representing whether the king passed in is in check
bool Board::inCheck(Piece &king, std::pair<char, int> currentPosition) {
  std::vector<std::pair<char, int>> allLegalMoves =
      this->generateThreatMap(&king);

  if ((king.getName() != 'k') && (king.getName() != 'K')) {
    return false;
  } else {
    for (auto move : allLegalMoves) {
      if (move == currentPosition) {
        return true;
      }
    }
  }

  return false;
}

// check if there is insufficient material to checkmate
bool Board::isInsufficientMaterial() {
  /*
    1. King vs King
    2. King and Bishop vs King
    3. King and Knight vs King
    5. King and Bishop vs King and Bishop
    6. King and Knight vs King and Knight
    7. King and Bishop vs King and Knight
  */

  // keep track of number of pieces
  int numBlackPawns = 0;
  int numBlackQueens = 0;
  int numBlackRook = 0;
  int numBlackBishops = 0;
  int numBlackKnights = 0;

  int numWhitePawns = 0;
  int numWhiteQueens = 0;
  int numWhiteRook = 0;
  int numWhiteBishops = 0;
  int numWhiteKnights = 0;

  // count number of pieces
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (currentBoard[i][j]->getName() == 'P') {
        ++numWhitePawns;
      } else if (currentBoard[i][j]->getName() == 'Q') {
        ++numWhiteQueens;
      } else if (currentBoard[i][j]->getName() == 'R') {
        ++numWhiteRook;
      } else if (currentBoard[i][j]->getName() == 'B') {
        ++numWhiteBishops;
      } else if (currentBoard[i][j]->getName() == 'N') {
        ++numWhiteKnights;
      } else if (currentBoard[i][j]->getName() == 'p') {
        ++numBlackPawns;
      } else if (currentBoard[i][j]->getName() == 'q') {
        ++numBlackQueens;
      } else if (currentBoard[i][j]->getName() == 'r') {
        ++numBlackRook;
      } else if (currentBoard[i][j]->getName() == 'b') {
        ++numBlackBishops;
      } else if (currentBoard[i][j]->getName() == 'n') {
        ++numBlackKnights;
      }
    }
  }

  // check if there is insufficient material

  // we only check if there are no queens, or rooks
  if (numBlackQueens != 0 || numWhiteQueens != 0 || numBlackRook != 0 ||
      numWhiteRook != 0) {
    return false;
  }

  // check if there are no pawns
  if (numBlackPawns != 0 || numWhitePawns != 0) {
    return false;
  }

  // if there are two bishops, its sufficient material
  if (numBlackBishops >= 2 || numWhiteBishops >= 2) {
    return false;
  }

  // if there is one bishop and one knight, its sufficient material
  if ((numBlackBishops == 1 && numBlackKnights >= 1) ||
      (numWhiteBishops == 1 && numWhiteKnights >= 1)) {
    return false;
  }

  // if there are no bishops and less than three knights, its insufficient
  // material
  if ((numBlackBishops == 0 && numBlackKnights < 3) ||
      (numWhiteBishops == 0 && numWhiteKnights < 3)) {
    return true;
  }

  // if there are no bishops and more than two knights, its sufficient material
  if ((numBlackBishops == 0 && numBlackKnights >= 3) ||
      (numWhiteBishops == 0 && numWhiteKnights >= 3)) {
    return false;
  }

  // if there are pieces besides the king, its insufficient material
  if ((numBlackBishops != 0 || numBlackKnights != 0) ||
      (numWhiteBishops != 0 || numWhiteKnights != 0)) {
    return true;
  }

  return true;
}

// function to check whether piece can be captured by opponent
bool Board::isPieceCapturable(Piece *p, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> allLegalMoves = this->generateThreatMap(p);

  for (auto move : allLegalMoves) {
    if (move == position) {
      return true;
    }
  }

  return false;
}


// if the move is not promotion, this is the handler function
  // supports castling and en passant, as well as normal moves
void Board::movePieceBase(std::pair<char, int> from, std::pair<char, int> to) {
  Piece *fromPiece = getPieceAtPosition(from);
  Piece *toPiece = getPieceAtPosition(to);

  // Castling
  if (fromPiece->getColor() == 'b' && from == std::make_pair('e', 8) &&
      to == std::make_pair('g', 8) && fromPiece->getName() == 'k' &&
      getPieceAtPosition(std::make_pair('h', 8))->getName() == 'r') {
    // delete whats on f8 and g8
    delete getPieceAtPosition(std::make_pair('f', 8));
    delete getPieceAtPosition(std::make_pair('g', 8));

    Piece *rook = getPieceAtPosition(std::make_pair('h', 8));
    setPieceAtPosition(std::make_pair('f', 8), rook);
    setPieceAtPosition(std::make_pair('h', 8), new NullPiece('*', '*'));
    setPieceAtPosition(std::make_pair('g', 8), fromPiece);
    setPieceAtPosition(std::make_pair('e', 8), new NullPiece('*', '*'));

    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;

  } else if (fromPiece->getColor() == 'b' && from == std::make_pair('e', 8) &&
             to == std::make_pair('c', 8) && fromPiece->getName() == 'k' &&
             getPieceAtPosition(std::make_pair('a', 8))->getName() == 'r') {
    // delete whats on b8, c8, and d8
    delete getPieceAtPosition(std::make_pair('b', 8));
    delete getPieceAtPosition(std::make_pair('c', 8));
    delete getPieceAtPosition(std::make_pair('d', 8));

    Piece *rook = getPieceAtPosition(std::make_pair('a', 8));
    setPieceAtPosition(std::make_pair('d', 8), rook);
    setPieceAtPosition(std::make_pair('a', 8), new NullPiece('*', '*'));
    setPieceAtPosition(std::make_pair('c', 8), fromPiece);
    setPieceAtPosition(std::make_pair('e', 8), new NullPiece('*', '*'));
    setPieceAtPosition(std::make_pair('b', 8), new NullPiece('*', '*'));

    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;

  } else if (fromPiece->getColor() == 'w' && from == std::make_pair('e', 1) &&
             to == std::make_pair('g', 1) && fromPiece->getName() == 'K' &&
             getPieceAtPosition(std::make_pair('h', 1))->getName() == 'R') {
    // delete whats on f1 and g1
    delete getPieceAtPosition(std::make_pair('f', 1));
    delete getPieceAtPosition(std::make_pair('g', 1));

    Piece *rook = getPieceAtPosition(std::make_pair('h', 1));
    setPieceAtPosition(std::make_pair('f', 1), rook);
    setPieceAtPosition(std::make_pair('h', 1), new NullPiece('*', '*'));
    setPieceAtPosition(std::make_pair('g', 1), fromPiece);
    setPieceAtPosition(std::make_pair('e', 1), new NullPiece('*', '*'));

    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;

  } else if (fromPiece->getColor() == 'w' && from == std::make_pair('e', 1) &&
             to == std::make_pair('c', 1) && fromPiece->getName() == 'K' &&
             getPieceAtPosition(std::make_pair('a', 1))->getName() == 'R') {
    // delete whats on b1, c1, and d1
    delete getPieceAtPosition(std::make_pair('b', 1));
    delete getPieceAtPosition(std::make_pair('c', 1));
    delete getPieceAtPosition(std::make_pair('d', 1));

    Piece *rook = getPieceAtPosition(std::make_pair('a', 1));
    setPieceAtPosition(std::make_pair('d', 1), rook);
    setPieceAtPosition(std::make_pair('a', 1), new NullPiece('*', '*'));
    setPieceAtPosition(std::make_pair('c', 1), fromPiece);
    setPieceAtPosition(std::make_pair('e', 1), new NullPiece('*', '*'));
    setPieceAtPosition(std::make_pair('b', 1), new NullPiece('*', '*'));

    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;

  }

  // En Passant
  if (enPassantPawn.first) {
    if (this->getPieceAtPosition(from)->getName() == 'P' &&
        (this->getPieceAtPosition(from)->getName() !=
         this->getPieceAtPosition(enPassantPawn)->getName())) {
      if (to.first == enPassantPawn.first &&
          to.second - 1 == enPassantPawn.second) {
        Piece *enpassant = getPieceAtPosition(enPassantPawn);
        delete toPiece;
        currentBoard[to.first - 'a'][to.second - 1] = fromPiece;
        delete enpassant;
        currentBoard[from.first - 'a'][from.second - 1] =
            new NullPiece{'*', '*'};
        currentBoard[enPassantPawn.first - 'a'][enPassantPawn.second - 1] =
            new NullPiece{'*', '*'};
        enPassantValid = false;
        enPassantMade = true;
        return;
      }
    } else if (this->getPieceAtPosition(from)->getName() == 'p' &&
               (this->getPieceAtPosition(from)->getName() !=
                this->getPieceAtPosition(enPassantPawn)->getName())) {
      if (to.first == enPassantPawn.first &&
          to.second + 1 == enPassantPawn.second) {
        Piece *enpassant = getPieceAtPosition(enPassantPawn);
        delete toPiece;
        currentBoard[to.first - 'a'][to.second - 1] = fromPiece;
        delete enpassant;
        currentBoard[from.first - 'a'][from.second - 1] =
            new NullPiece{'*', '*'};
        currentBoard[enPassantPawn.first - 'a'][enPassantPawn.second - 1] =
            new NullPiece{'*', '*'};
        enPassantValid = false;
        enPassantMade = true;
        return;
      }
    }
  }

  if (fromPiece->getName() != '*' &&
      (fromPiece->getColor() != toPiece->getColor())) {
    // if the piece is NullPiece or opponent piece...
    delete toPiece;

    // move the piece
    currentBoard[to.first - 'a'][to.second - 1] = fromPiece;

    if (fromPiece->getName() == 'k') {
      this->setBlackKingPosition(to);
    } else if (fromPiece->getName() == 'K') {
      this->setWhiteKingPosition(to);
    }

    this->getPieceAtPosition(to)->setPieceAsMoved();

    if (abs(from.second - to.second) == 2 &&
        (getPieceAtPosition(to)->getName() == 'P' ||
         getPieceAtPosition(to)->getName() == 'p')) {
      setEnPassantPawn(to);
      enPassantValid = true;
    } else {
      enPassantValid = false;
    }

    // set old position to a new null piece
    currentBoard[from.first - 'a'][from.second - 1] = new NullPiece{'*', '*'};
  }
}


// entrance function which does checks to make sure that the move that is trying
// to be made is legal. If it is, it will call the corresponding movePieceBase
// function
bool Board::movePiece(std::pair<char, int> from, std::pair<char, int> to) {
  Piece *currentPiece = this->getPieceAtPosition(from);
  this->parsePossibleMoves(*currentPiece, from);

  // checks if when castling, the king doesn't move through a square that is
  // under attack
  if (currentPiece->getName() == 'k' || currentPiece->getName() == 'K') {
    if (currentPiece->getColor() == 'w') {
      if (from.first == 'e' && from.second == 1) {
        if (to.first == 'g' && to.second == 1 && !currentPiece->getHasMoved()) {
          if (this->getPieceAtPosition(std::make_pair('h', 1))->getName() !=
              'R') {
            return false;
          }
          // move king to e1, f1, and g1 on a copy of the board, and check if
          // any of them put the king in check

          // first check if f1 and g1 are empty
          if (this->getPieceAtPosition(std::make_pair('f', 1))->getName() !=
                  '*' ||
              this->getPieceAtPosition(std::make_pair('g', 1))->getName() !=
                  '*') {
            return false;
          }

          // generate threat map to see if when the king moves to f1 or g1,
          // it's in check
          std::vector<std::pair<char, int>> threatMap =
              this->generateThreatMap(this->getWhiteKing());

          // check if f1 or g1 are in the threat map
          for (auto move : threatMap) {
            if (move == std::make_pair('f', 1) ||
                move == std::make_pair('g', 1)) {
              return false;
            }
          }
        } else if (to.first == 'c' && to.second == 1 &&
                   !currentPiece->getHasMoved()) {
          if (this->getPieceAtPosition(std::make_pair('a', 1))->getName() !=
              'R') {
            return false;
          }

          // move king to e1, d1, and c1 on a copy of the board, and check if
          // any of them put the king in check

          // first check if d1, c1, and b1 are empty
          if (this->getPieceAtPosition(std::make_pair('d', 1))->getName() !=
                  '*' ||
              this->getPieceAtPosition(std::make_pair('c', 1))->getName() !=
                  '*' ||
              this->getPieceAtPosition(std::make_pair('b', 1))->getName() !=
                  '*') {
            return false;
          }

          // generate threat map to see if when the king moves to d1 or c1,
          // it's in check
          std::vector<std::pair<char, int>> threatMap =
              this->generateThreatMap(this->getWhiteKing());

          // check if d1 or c1 are in the threat map
          for (auto move : threatMap) {
            if (move == std::make_pair('d', 1) ||
                move == std::make_pair('c', 1)) {
              return false;
            }
          }
        }
      }
    } else {
      if (from.first == 'e' && from.second == 8) {
        if (to.first == 'g' && to.second == 8 && !currentPiece->getHasMoved()) {
          if (this->getPieceAtPosition(std::make_pair('h', 8))->getName() !=
              'r') {
            return false;
          }
          // move king to e8, f8, and g8 on a copy of the board, and check if
          // any of them put the king in check

          // first check if f8 and g8 are empty
          if (this->getPieceAtPosition(std::make_pair('f', 8))->getName() !=
                  '*' ||
              this->getPieceAtPosition(std::make_pair('g', 8))->getName() !=
                  '*') {
            return false;
          }

          // generate threat map to see if when the king moves to f8 or g8,
          // it's in check
          std::vector<std::pair<char, int>> threatMap =
              this->generateThreatMap(this->getBlackKing());

          // check if f8 or g8 are in the threat map
          for (auto move : threatMap) {
            if (move == std::make_pair('f', 8) ||
                move == std::make_pair('g', 8)) {
              return false;
            }
          }
        } else if (to.first == 'c' && to.second == 8 &&
                   !currentPiece->getHasMoved()) {
          if (this->getPieceAtPosition(std::make_pair('a', 8))->getName() !=
              'r') {
            return false;
          }
          // move king to e8, d8, and c8 on a copy of the board, and check if
          // any of them put the king in check

          // first check if d8, c8, and b8 are empty
          if (this->getPieceAtPosition(std::make_pair('d', 8))->getName() !=
                  '*' ||
              this->getPieceAtPosition(std::make_pair('c', 8))->getName() !=
                  '*' ||
              this->getPieceAtPosition(std::make_pair('b', 8))->getName() !=
                  '*') {
            return false;
          }

          // generate threat map to see if when the king moves to d8 or c8,
          // it's in check
          std::vector<std::pair<char, int>> threatMap =
              this->generateThreatMap(this->getBlackKing());

          // check if d8 or c8 are in the threat map
          for (auto move : threatMap) {
            if (move == std::make_pair('d', 8) ||
                move == std::make_pair('c', 8)) {
              return false;
            }
          }
        }
      }
    }
  }

  for (auto move : currentPiece->allPossibleMoves) {
    if (move == to) {
      Board *tmpBoard = this->clone();

      tmpBoard->movePieceBase(from, to);

      if (currentPiece->getColor() == 'b') {
        if (tmpBoard->inCheck(*(tmpBoard->getBlackKing()),
                              tmpBoard->getBlackKingPosition()) == false) {
          this->movePieceBase(from, to);
          delete tmpBoard;
          return true;
        } else {
          delete tmpBoard;
          return false;
        }
      } else if (currentPiece->getColor() == 'w') {
        if (tmpBoard->inCheck(*(tmpBoard->getWhiteKing()),
                              tmpBoard->getWhiteKingPosition()) == false) {
          this->movePieceBase(from, to);
          delete tmpBoard;
          return true;
        } else {
          delete tmpBoard;
          return false;
        }
      }
      delete tmpBoard;
    }
  }
  // if all this fails, something bad happened, return false
  return false;
}

// overloaded movePieceBase function for when the player is promoting a pawn
bool Board::movePieceBase(std::pair<char, int> oldPosition,
                          std::pair<char, int> newPosition, char promote) {
  Piece *currentPiece = this->getPieceAtPosition(oldPosition);
  Board *tmpBoard = this->clone();

  tmpBoard->movePieceBase(oldPosition, newPosition);

  if (currentPiece->getColor() == 'b') {
    if (tmpBoard->inCheck(*(tmpBoard->getBlackKing()),
                          tmpBoard->getBlackKingPosition()) == false) {
      Piece *caputerdPiece = getPieceAtPosition(newPosition);
      delete caputerdPiece;
      delete getPieceAtPosition(oldPosition);
      currentBoard[newPosition.first - 'a'][newPosition.second - 1] =
          createPiece(promote);

      currentBoard[oldPosition.first - 'a'][oldPosition.second - 1] =
          new NullPiece{'*', '*'};
      // set rook and king as moved
      delete tmpBoard;
      return true;
    } else {
      delete tmpBoard;
      return false;
    }
  } else if (currentPiece->getColor() == 'w') {
    if (tmpBoard->inCheck(*(tmpBoard->getWhiteKing()),
                          tmpBoard->getWhiteKingPosition()) == false) {
      Piece *caputerdPiece = getPieceAtPosition(newPosition);
      delete caputerdPiece;
      delete getPieceAtPosition(oldPosition);
      currentBoard[newPosition.first - 'a'][newPosition.second - 1] =
          createPiece(promote);

      currentBoard[oldPosition.first - 'a'][oldPosition.second - 1] =
          new NullPiece{'*', '*'};
      delete tmpBoard;
      return true;
    } else {
      delete tmpBoard;
      return false;
    }
  }
  delete tmpBoard;
  return false;
}

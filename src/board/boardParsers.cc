#include "board.h"

// given the current state of a board, recalculate all possible moves for all
// pieces on the board
void Board::generateCompleteMoves() {
  std::pair<char, int> position;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      position = std::make_pair(this->convertNumToAlpha(x), y + 1);

      // get all moves and then parse.
      if (this->currentBoard[x][y]->getName() == 'k' ||
          this->currentBoard[x][y]->getName() == 'K' ||
          this->currentBoard[x][y]->getName() == 'p' ||
          this->currentBoard[x][y]->getName() == 'P' ||
          this->currentBoard[x][y]->getName() == 'n' ||
          this->currentBoard[x][y]->getName() == 'N') {
        this->currentBoard[x][y]->getAllPossibleMoves(position);
      };

      if (this->currentBoard[x][y]->getName() != 'k' &&
          this->currentBoard[x][y]->getName() != 'K') {
        this->parsePossibleMoves(*(this->currentBoard[x][y]), position);
      } else {
        if (this->currentBoard[x][y]->getColor() == 'w') {
          this->whiteKingPosition = position;
        } else {
          this->blackKingPosition = position;
        }
      }
    }
  }

  if (this->blackKing) {
    this->parsePossibleMoves(*(this->blackKing), this->blackKingPosition);
  }

  if (this->whiteKing) {
    this->parsePossibleMoves(*(this->whiteKing), this->whiteKingPosition);
  }
}

// generates all the possible squares that the opponent can move to
std::vector<std::pair<char, int>> Board::generateThreatMap(Piece *p) {
  std::vector<std::pair<char, int>> tmp;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (this->currentBoard[x][y]->getName() != '*') {
        if (this->currentBoard[x][y]->getColor() != p->getColor()) {
          this->currentBoard[x][y]->getAllPossibleMoves(
              std::make_pair(this->convertNumToAlpha(x), y + 1));

          if (this->currentBoard[x][y]->getName() != 'k' &&
              this->currentBoard[x][y]->getName() != 'K' &&
              this->currentBoard[x][y]->getName() != 'p' &&
              this->currentBoard[x][y]->getName() != 'P') {
            this->parsePossibleMoves(
                *(this->currentBoard[x][y]),
                std::make_pair(this->convertNumToAlpha(x), y + 1));
          }

          for (auto move : this->currentBoard[x][y]->allPossibleMoves) {
            if (this->currentBoard[x][y]->getName() == 'p' ||
                this->currentBoard[x][y]->getName() == 'P') {
              // ignore forward moves in pawns (they're not threats)
              if (move.first != convertNumToAlpha(x)) {
                tmp.push_back(move);
              }
            } else {
              tmp.push_back(move);
            }
          }
        }
      }
    }
  }

  return tmp;
}


// given a board, a piece, and it's current position, parse all the possible
// legal moves for that piece, modifying the possible moves vector for the piece
void Board::parsePossibleMoves(Piece &piece, std::pair<char, int> position) {
  // pawn
  if (piece.getName() == 'p' || piece.getName() == 'P') {
    parsePossibleMovesPawn(piece, position);
  }
  // rook
  else if (piece.getName() == 'r' || piece.getName() == 'R') {
    parsePossibleMovesRook(piece, position);
  }
  // knight
  else if (piece.getName() == 'n' || piece.getName() == 'N') {
    parsePossibleMovesKnight(piece, position);
  }
  // bishop
  else if (piece.getName() == 'b' || piece.getName() == 'B') {
    parsePossibleMovesBishop(piece, position);
  }
  // queen
  else if (piece.getName() == 'q' || piece.getName() == 'Q') {
    parsePossibleMovesQueen(piece, position);
  }
  // king
  else if (piece.getName() == 'k' || piece.getName() == 'K') {
    parsePossibleMovesKing(piece, position);
  }
}

// parsePossibleMoves for a rook
void Board::parsePossibleMovesRook(Piece &rook, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;
  int x = position.first;
  int y = position.second;

  std::pair<char, int> newMove;

  // move left
  while (x > 'a') {
    x -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != rook.getColor()) {
      tmp.push_back(newMove);

      break;
    } else if (tmpPiece->getColor() == rook.getColor()) {
      break;
    }
  }

  x = position.first;

  // move right
  while (x < 'h') {
    x += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != rook.getColor()) {
      tmp.push_back(newMove);

      break;
    } else if (tmpPiece->getColor() == rook.getColor()) {
      break;
    }
  }

  x = position.first;

  // move down
  while (y > 1) {
    y -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != rook.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == rook.getColor()) {
      break;
    }
  }

  y = position.second;

  // move up
  while (y < 8) {
    y += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != rook.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == rook.getColor()) {
      break;
    }
  }

  // add all possible moves to the piece
  rook.allPossibleMoves = tmp;
}

// parsePossibleMoves for a bishop
void Board::parsePossibleMovesBishop(Piece &bishop,
                                     std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;
  /* Bishop movement */
  int x = position.first;
  int y = position.second;

  std::pair<char, int> newMove;

  // move diagonally left and down
  while (x > 'a' && y > 1) {
    x -= 1;
    y -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != bishop.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == bishop.getColor()) {
      break;
    }
  }

  x = position.first;
  y = position.second;

  // move diagonally left and up
  while (x > 'a' && y < 8) {
    x -= 1;
    y += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != bishop.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == bishop.getColor()) {
      break;
    }
  }

  x = position.first;
  y = position.second;

  // move diagonally right and up
  while (x < 'h' && y < 8) {
    x += 1;
    y += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != bishop.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == bishop.getColor()) {
      break;
    }
  }

  x = position.first;
  y = position.second;

  // move diagonally right and down
  while (x < 'h' && y > 1) {
    x += 1;
    y -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != bishop.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == bishop.getColor()) {
      break;
    }
  }

  // set possible moves for bishop
  bishop.allPossibleMoves = tmp;
}

// parsePossibleMoves for a knight
void Board::parsePossibleMovesKnight(Piece &knight, std::pair<char, int>) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : knight.allPossibleMoves) {
    // as long as the piece at 'move' is not the same color, its a valid move
    if (this->getPieceAtPosition(move)->getColor() != knight.getColor()) {
      tmp.push_back(move);
    }
  }
  knight.allPossibleMoves = tmp;
}

// parsePossibleMoves for a pawn
void Board::parsePossibleMovesPawn(Piece &pawn, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : pawn.allPossibleMoves) {
    // if piece has moved before, it can only move one space

    // diagonal moves have a diff. x coordinate and a diff. y coordinate
    if ((move.first != position.first) && (move.second != position.second)) {
      // if the square is not empty and it is the opponents piece, its a
      // valid move.

      // Here we will also need to check if moving the pawn causes check to
      // its own king.
      if (this->getPieceAtPosition(move)->getColor() == '*' &&
          move.first == enPassantPawn.first &&
          move.second - 1 == enPassantPawn.second && enPassantValid &&
          getPieceAtPosition(enPassantPawn)->getName() != pawn.getName() &&
          pawn.getColor() == 'w') {
        tmp.push_back(move);
      } else if (this->getPieceAtPosition(move)->getColor() == '*' &&
                 move.first == enPassantPawn.first &&
                 move.second + 1 == enPassantPawn.second && enPassantValid &&
                 getPieceAtPosition(enPassantPawn)->getName() !=
                     pawn.getName() &&
                 pawn.getColor() == 'b') {
        tmp.push_back(move);
      } else if (this->getPieceAtPosition(move)->getColor() !=
                     pawn.getColor() &&
                 this->getPieceAtPosition(move)->getColor() != '*') {
        tmp.push_back(move);
      }
    }  // forward moves only have a diff. y coordinate
    else if (move.second != position.second && move.first == position.first) {
      // if the move is two squares forward, we need to check if the square
      // in between is empty
      if (abs(move.second - position.second) == 2) {
        // get the move in between
        std::pair<char, int> inBetweenMove;

        // if the pawn is white, the move is up
        if (pawn.getColor() == 'w') {
          inBetweenMove = std::make_pair(move.first, move.second - 1);
        } else {
          inBetweenMove = std::make_pair(move.first, move.second + 1);
        }

        // if the square in between is empty, its a valid move
        if (this->getPieceAtPosition(inBetweenMove)->getName() == '*' &&
            this->getPieceAtPosition(move)->getName() == '*') {
          tmp.push_back(move);
        }
      } else if (abs(move.second - position.second) == 1) {
        // if the square is empty, its a valid move
        if (this->getPieceAtPosition(move)->getName() == '*') {
          tmp.push_back(move);
        }
      }
    }
  }
  pawn.allPossibleMoves = tmp;
}

// parsePossibleMoves for a king
void Board::parsePossibleMovesKing(Piece &king, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;
  std::vector<std::pair<char, int>> threatMap = this->generateThreatMap(&king);

  bool shouldAdd = true;

  for (auto move : king.allPossibleMoves) {
    shouldAdd = true;

    if (king.getColor() != this->getPieceAtPosition(move)->getColor()) {
      // move the King to this potential position
      for (auto threat : threatMap) {
        if (move == threat) {
          shouldAdd = false;
        }
      }

      if (shouldAdd == true) {
        /* We should check if the move causes check to the King.
          If so, its invalid! */
        Board *tmpBoard = this->clone();

        tmpBoard->movePieceBase(position, move);

        if (king.getColor() == 'b') {
          if (tmpBoard->inCheck(*(tmpBoard->getBlackKing()),
                                tmpBoard->getBlackKingPosition()) == false) {
            tmp.push_back(move);
          }
        } else if (king.getColor() == 'w') {
          if (tmpBoard->inCheck(*(tmpBoard->getWhiteKing()),
                                tmpBoard->getWhiteKingPosition()) == false) {
            tmp.push_back(move);
          }
        }

        delete tmpBoard;
      }
    }
  }

  king.allPossibleMoves = tmp;
}

// parsePossibleMoves for a queen
void Board::parsePossibleMovesQueen(Piece &queen,
                                    std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  // for each direction within the bounds of the board, check if the space is
  // empty or occupied by an enemy piece. If it is empty, add it to the list of
  // possible moves. If it is occupied by an enemy piece, add it to the list of
  // possible moves and stop checking in that direction. If it is occupied by a
  // friendly piece, stop checking in that direction.

  /* Bishop movement */
  int x = position.first;
  int y = position.second;

  std::pair<char, int> newMove;

  // move diagonally left and down
  while (x > 'a' && y > 1) {
    x -= 1;
    y -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);

      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  x = position.first;
  y = position.second;

  // move diagonally left and up
  while (x > 'a' && y < 8) {
    x -= 1;
    y += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  x = position.first;
  y = position.second;

  // move diagonally right and up
  while (x < 'h' && y < 8) {
    x += 1;
    y += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  x = position.first;
  y = position.second;

  // move diagonally right and down
  while (x < 'h' && y > 1) {
    x += 1;
    y -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  /* Rook movement */
  x = position.first;
  y = position.second;

  // move left
  while (x > 'a') {
    x -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  x = position.first;

  // move right
  while (x < 'h') {
    x += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  x = position.first;

  // move down
  while (y > 1) {
    y -= 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  y = position.second;

  // move up
  while (y < 8) {
    y += 1;

    newMove = std::make_pair(x, y);
    Piece *tmpPiece = getPieceAtPosition(newMove);

    if (tmpPiece->getName() == '*') {
      tmp.push_back(newMove);
    } else if (tmpPiece->getColor() != queen.getColor()) {
      tmp.push_back(newMove);
      break;
    } else if (tmpPiece->getColor() == queen.getColor()) {
      break;
    }
  }

  queen.allPossibleMoves = tmp;
}

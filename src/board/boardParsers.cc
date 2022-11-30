#include "board.h"

void Board::parsePossibleMoves(Piece &piece, std::pair<char, int> position) {
  // King
  if (piece.getName() == 'k' || piece.getName() == 'K') {
    parsePossibleMovesKing(piece, position);
  }
  // Knight
  else if (piece.getName() == 'n' || piece.getName() == 'N') {
    parsePossibleMovesKnight(piece, position);
  }
  // Pawn
  else if (piece.getName() == 'p' || piece.getName() == 'P') {
    parsePossibleMovesPawn(piece, position);
  }
  // Queen
  else if (piece.getName() == 'q' || piece.getName() == 'Q') {
    parsePossibleMovesQueen(piece, position);
  }
  // Rook
  else if (piece.getName() == 'r' || piece.getName() == 'R') {
    parsePossibleMovesRook(piece, position);
  }
  // Bishop
  else if (piece.getName() == 'b' || piece.getName() == 'B') {
    parsePossibleMovesBishop(piece, position);
  }
}

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

void Board::parsePossibleMovesKnight(Piece &knight,
                                     std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : knight.allPossibleMoves) {
    // as long as the piece at 'move' is not the same color, its a valid move
    if (this->getPieceAtPosition(move)->getColor() != knight.getColor()) {
      tmp.push_back(move);
    }
  }

  knight.allPossibleMoves = tmp;
}

void Board::parsePossibleMovesPawn(Piece &pawn, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : pawn.allPossibleMoves) {
    // diagonal moves have a diff. x coordinate and a diff. y coordinate
    if ((move.first != position.first) && (move.second != position.second)) {
      // if the square is not empty and it is the opponents piece, its a
      // valid move.

      // Here we will also need to check if moving the pawn causes check to
      // its own king.
      if (this->getPieceAtPosition(move)->getColor() != pawn.getColor() &&
          this->getPieceAtPosition(move)->getColor() != '*') {
        tmp.push_back(move);
      }
    } // forward moves only have a diff. y coordinate
    else if (move.second != position.second) {
      // if the square is empty then only we can move
      if (this->getPieceAtPosition(move)->getColor() == '*') {
        tmp.push_back(move);
      }
    }
  }

  pawn.allPossibleMoves = tmp;
}

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
      break;
    } else if (tmpPiece->getColor() == rook.getColor()) {
      break;
    }
  }

  // add all possible moves to the piece
  rook.allPossibleMoves = tmp;
}

// bishop move parser
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

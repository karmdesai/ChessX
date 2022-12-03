#include "board.h"

#include <iostream>
#include <vector>

#include "../pieces/bishop.h"
#include "../pieces/king.h"
#include "../pieces/knight.h"
#include "../pieces/nullPiece.h"
#include "../pieces/pawn.h"
#include "../pieces/piece.h"
#include "../pieces/queen.h"
#include "../pieces/rook.h"

using namespace std;

/* Board class */
Board::Board() {
  // start with board of nullpieces
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      this->currentBoard[x][y] = new NullPiece('*', '*');
    }
  }

  this->whosColourTurn = 'w';
}

// Destructor
Board::~Board() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      delete this->currentBoard[x][y];
    }
  }
}

void Board::defaultInitialization() {
  // Initialize white pieces
  delete this->currentBoard[0][0];
  delete this->currentBoard[1][0];
  delete this->currentBoard[2][0];
  delete this->currentBoard[3][0];
  delete this->currentBoard[4][0];
  delete this->currentBoard[5][0];
  delete this->currentBoard[6][0];
  delete this->currentBoard[7][0];

  this->currentBoard[0][0] = new Rook('R', 'w', true);
  this->currentBoard[1][0] = new Knight('N', 'w');
  this->currentBoard[2][0] = new Bishop('B', 'w');
  this->currentBoard[3][0] = new Queen('Q', 'w');

  Piece *newWhiteKing = new King('K', 'w', true);
  this->whiteKing = newWhiteKing;
  this->currentBoard[4][0] = newWhiteKing;
  this->whiteKingPosition = std::make_pair('e', 1);

  this->currentBoard[5][0] = new Bishop('B', 'w');
  this->currentBoard[6][0] = new Knight('N', 'w');
  this->currentBoard[7][0] = new Rook('R', 'w', true);

  for (int i = 0; i < 8; i++) {
    delete this->currentBoard[i][1];
    this->currentBoard[i][1] = new Pawn('P', 'w', true);
  }

  // Initialize black pieces
  delete this->currentBoard[0][7];
  delete this->currentBoard[1][7];
  delete this->currentBoard[2][7];
  delete this->currentBoard[3][7];
  delete this->currentBoard[4][7];
  delete this->currentBoard[5][7];
  delete this->currentBoard[6][7];
  delete this->currentBoard[7][7];

  this->currentBoard[0][7] = new Rook('r', 'b', true);
  this->currentBoard[1][7] = new Knight('n', 'b');
  this->currentBoard[2][7] = new Bishop('b', 'b');
  this->currentBoard[3][7] = new Queen('q', 'b');

  Piece *newBlackKing = new King('k', 'b', true);
  this->blackKing = newBlackKing;
  this->currentBoard[4][7] = newBlackKing;
  this->blackKingPosition = std::make_pair('e', 8);

  this->currentBoard[5][7] = new Bishop('b', 'b');
  this->currentBoard[6][7] = new Knight('n', 'b');
  this->currentBoard[7][7] = new Rook('r', 'b', true);

  for (int i = 0; i < 8; i++) {
    delete this->currentBoard[i][6];
    this->currentBoard[i][6] = new Pawn('p', 'b', true);
  }

  this->generateCompleteMoves();
}

// Overload the << operator for the Board class
std::ostream &operator<<(std::ostream &out, const Board *myBoard) {
  for (int y = 7; y >= 0; y--) {
    out << y + 1 << " ";

    for (int x = 0; x < 8; x++) {
      Piece *currentSpace = myBoard->currentBoard[x][y];

      if (currentSpace->getName() != '*') {
        out << currentSpace->getName();
      } else {
        // a even sum of coordinates is a black square
        if ((x + y) % 2 == 0) {
          out << "_";
        }
        // an odd sum of coordinates is a white square
        else {
          out << " ";
        }
      }
    }

    out << std::endl;
  }

  // print x-axis as letters
  out << std::endl;
  out << "  abcdefgh";
  out << std::endl;

  return out;
}

// convertAlphaToNum() converts a character ranging from a to h
//  to a number ranging from 0 to 7.
// requires:
//  - alpha is a character ranging from a to h.
// time: O(1)
int Board::convertAlphaToNum(char alpha) {
  if (alpha >= 'a' && alpha <= 'h') {
    return alpha - 'a';
  } else {
    return -1;
  }
}

// convertNumToAlpha() converts a number ranging from 0 to 7
//  to a character ranging from a to h.
// requires:
//  - num is an integer ranging from 0 to 7.
// time: O(1)
char Board::convertNumToAlpha(int num) {
  if (num >= 0 && num <= 7) {
    return num + 'a';
  } else {
    return '*';
  }
}

// createPiece(c) takes a character c and creates a new Piece
//  of name c. It returns a pointer to the newly created piece.
// Requires:
//  - c is a valid character
// Time: O(1)
Piece *Board::createPiece(char c) {
  if (c == 'R') {
    return new Rook('R', 'w', false);
  } else if (c == 'N') {
    return new Knight('N', 'w');
  } else if (c == 'B') {
    return new Bishop('B', 'w');
  } else if (c == 'Q') {
    return new Queen('Q', 'w');
  } else if (c == 'K') {
    return new King('K', 'w', false);
  } else if (c == 'P') {
    return new Pawn('P', 'w', false);
  } else if (c == 'r') {
    return new Rook('r', 'b', false);
  } else if (c == 'n') {
    return new Knight('n', 'b');
  } else if (c == 'b') {
    return new Bishop('b', 'b');
  } else if (c == 'q') {
    return new Queen('q', 'b');
  } else if (c == 'k') {
    return new King('k', 'b', false);
  } else if (c == 'p') {
    return new Pawn('p', 'b', false);
  } else {
    return new NullPiece('*', '*');
  }
}

// clone() creates a new Board that is a deep copy of the current Board.
Board *Board::clone() {
  Board *newBoard = new Board();

  for (int x = 0; x < 8; ++x) {
    for (int y = 0; y < 8; ++y) {
      delete newBoard->currentBoard[x][y];
      newBoard->currentBoard[x][y] = this->currentBoard[x][y]->clone();

      // set BlackKing and WhiteKing fields.
      if (newBoard->currentBoard[x][y]->getName() == 'k') {
        newBoard->blackKing = newBoard->currentBoard[x][y];
      } else if (newBoard->currentBoard[x][y]->getName() == 'K') {
        newBoard->whiteKing = newBoard->currentBoard[x][y];
      }
    }
  }

  newBoard->whosColourTurn = this->whosColourTurn;
  newBoard->whiteKingPosition = this->whiteKingPosition;
  newBoard->blackKingPosition = this->blackKingPosition;

  return newBoard;
}

/* Start Getters */
Piece *Board::getPieceAtPosition(std::pair<char, int> position) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  /* Check if position is out of bounds first?

  if (x == -1 || y < 1 || y > 8) {
      return false;
  }
  */

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  return this->currentBoard[x][y];
}

Piece *Board::getWhiteKing() { return this->whiteKing; }

Piece *Board::getBlackKing() { return this->blackKing; }

std::pair<char, int> Board::getWhiteKingPosition() {
  return this->whiteKingPosition;
}

std::pair<char, int> Board::getBlackKingPosition() {
  return this->blackKingPosition;
}
/* End Getters */

/* Start Setters */
void Board::setPieceAtPosition(std::pair<char, int> position, Piece *p) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;
  /* Check if position is out of bounds first?

  if (x == -1 || y < 1 || y > 8) {
      return false;
  }
  */

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  this->currentBoard[x][y] = p;
}

void Board::setColourTurn(char player) {
  if (player == 'w' || player == 'b') {
    this->whosColourTurn = player;
  }
}

void Board::setWhiteKing(Piece *wk) { this->whiteKing = wk; }

void Board::setBlackKing(Piece *bk) { this->blackKing = bk; }

void Board::setWhiteKingPosition(std::pair<char, int> position) {
  this->whiteKingPosition = position;
}

void Board::setBlackKingPosition(std::pair<char, int> position) {
  this->blackKingPosition = position;
}
/* End Setters */

char Board::getColourTurn() { return this->whosColourTurn; }

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

void Board::parsePossibleMovesKnight(Piece &knight, std::pair<char, int>) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : knight.allPossibleMoves) {
    // as long as the piece at 'move' is not the same color, its a valid move
    if (this->getPieceAtPosition(move)->getColor() != knight.getColor()) {
      tmp.push_back(move);
    }
  }
  /* this is not ideal, we should have Piece.allPossibleMoves is a pointer to
    the vector, and tmp is a pointer to a vector. Then we can just swap the
    memory of the two vectors for optimal performance. */
  knight.allPossibleMoves = tmp;
}

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
  /* this is not ideal, we should have Piece.allPossibleMoves is a pointer to
    the vector, and tmp is a pointer to a vector. Then we can just swap the
    memory of the two vectors for optimal performance. */
  pawn.allPossibleMoves = tmp;
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

// queen move parser
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

// Brutally inefficient, there are better ways to do this and
//  usually you only need to re-calculate the parsed move list for certain
//  pieces.
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

std::vector<std::pair<char, int>> Board::generateThreatMap(Piece *p) {
  std::vector<std::pair<char, int>> tmp;

  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if (this->currentBoard[x][y]->getName() != '*') {
        if (this->currentBoard[x][y]->getColor() != p->getColor()) {
          this->currentBoard[x][y]->getAllPossibleMoves(
              std::make_pair(this->convertNumToAlpha(x), y + 1));

          /* If we allow parsePossibleMoves for King:
              - We get a segmentation fault (due to infinite recursion)
              If we allow parsePossibleMoves for Pawn:
              - The parser will remove all diagonal moves (since those
                squares are empty). But we don't want the King to move
                to those squares, since the Pawn can capture it.
              - Note that the King won't be able to move there (since we
                implemented the tmpBoard checking functionality), but the
                movelist will allow these moves.
          */
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

bool Board::inCheck(Piece &king, std::pair<char, int> currentPosition) {
  std::vector<std::pair<char, int>> allLegalMoves =
      this->generateThreatMap(&king);

  if ((king.getName() != 'k') && (king.getName() != 'K')) {
    return false;
  } else {
    // O(n^3) efficiency 💀 let's think of some optimization later.
    for (auto move : allLegalMoves) {
      if (move == currentPosition) {
        return true;
      }
    }
  }

  return false;
}

bool Board::movePiece(std::pair<char, int> from, std::pair<char, int> to) {
  /* REMOVING THIS LINE REMOVES THE SEGFAULT, BUT THEN
    THE PROGRAM DOESN'T GENERATE ANY MOVES PAST THE FIRST ONE. (try with
    main.in)
  */
  // generateCompleteMoves();

  Piece *currentPiece = this->getPieceAtPosition(from);
  this->parsePossibleMoves(*currentPiece, from);

  // checks if when castling, the king doesn't move through a square that is
  // under attack
  if (currentPiece->getName() == 'k' || currentPiece->getName() == 'K') {
    if (currentPiece->getColor() == 'w') {
      if (from.first == 'e' && from.second == 1) {
        if (to.first == 'g' && to.second == 1) {
          // move king to e1, f1, and g1 on a copy of the board, and check if
          // any of them put the king in check
          Board *tmpBoard = this->clone();
          tmpBoard->movePieceBase(from, std::make_pair('f', 1));
          if (tmpBoard->inCheck(*(tmpBoard->whiteKing),
                                tmpBoard->whiteKingPosition)) {
          delete tmpBoard;
            return false;
          }
          tmpBoard->movePieceBase(std::make_pair('f', 1),
                                  std::make_pair('g', 1));
          if (tmpBoard->inCheck(*(tmpBoard->whiteKing),
                                tmpBoard->whiteKingPosition)) {
          delete tmpBoard;
            return false;
          }
          delete tmpBoard;
        } else if (to.first == 'c' && to.second == 1) {
          // move king to e1, d1, and c1 on a copy of the board, and check if
          // any of them put the king in check
          Board *tmpBoard = this->clone();
          tmpBoard->movePieceBase(from, std::make_pair('d', 1));
          if (tmpBoard->inCheck(*(tmpBoard->whiteKing),
                                tmpBoard->whiteKingPosition)) {

          delete tmpBoard;
            return false;
          }
          tmpBoard->movePieceBase(std::make_pair('d', 1),
                                  std::make_pair('c', 1));
          if (tmpBoard->inCheck(*(tmpBoard->whiteKing),
                                tmpBoard->whiteKingPosition)) {
          delete tmpBoard;
            return false;
          }
          delete tmpBoard;
        }
      }
    } else {
      if (from.first == 'e' && from.second == 8) {
        if (to.first == 'g' && to.second == 8) {
          // move king to e8, f8, and g8 on a copy of the board, and check if
          // any of them put the king in check
          Board *tmpBoard = this->clone();
          tmpBoard->movePieceBase(from, std::make_pair('f', 8));
          if (tmpBoard->inCheck(*(tmpBoard->blackKing),
                                tmpBoard->blackKingPosition)) {
          delete tmpBoard;
            return false;
          }
          tmpBoard->movePieceBase(std::make_pair('f', 8),
                                  std::make_pair('g', 8));
          if (tmpBoard->inCheck(*(tmpBoard->blackKing),
                                tmpBoard->blackKingPosition)) {
          delete tmpBoard;
            return false;
          }
          delete tmpBoard;
        } else if (to.first == 'c' && to.second == 8) {
          // move king to e8, d8, and c8 on a copy of the board, and check if
          // any of them put the king in check
          Board *tmpBoard = this->clone();
          tmpBoard->movePieceBase(from, std::make_pair('d', 8));
          if (tmpBoard->inCheck(*(tmpBoard->blackKing),
                                tmpBoard->blackKingPosition)) {
          delete tmpBoard;
            return false;
          }
          tmpBoard->movePieceBase(std::make_pair('d', 8),
                                  std::make_pair('c', 8));
          if (tmpBoard->inCheck(*(tmpBoard->blackKing),
                                tmpBoard->blackKingPosition)) {
          delete tmpBoard;
            return false;
          }
          delete tmpBoard;
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

void Board::movePieceBase(std::pair<char, int> from, std::pair<char, int> to) {
  Piece *fromPiece = getPieceAtPosition(from);
  Piece *toPiece = getPieceAtPosition(to);

  // Castling
  if (fromPiece->getColor() == 'b' && to == std::make_pair('g', 8) &&
      fromPiece->getName() == 'k' &&
      getPieceAtPosition(std::make_pair('h', 8))->getName() == 'r') {
    Piece *rook = getPieceAtPosition(std::make_pair('h', 8));
    delete toPiece;

    currentBoard[to.first - 'a'][to.second - 1] = fromPiece;
    currentBoard[to.first - 1 - 'a'][to.second - 1] = rook;

    currentBoard[from.first - 'a'][from.second - 1] = new NullPiece{'*', '*'};
    currentBoard['h' - 'a'][7] = new NullPiece{'*', '*'};
    // set rook and king as moved
    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;

  } else if (fromPiece->getColor() == 'b' && to == std::make_pair('c', 8) &&
             fromPiece->getName() == 'k' &&
             getPieceAtPosition(std::make_pair('a', 8))->getName() == 'r') {
    Piece *rook = getPieceAtPosition(std::make_pair('a', 8));
    delete toPiece;

    currentBoard[to.first - 'a'][to.second - 1] = fromPiece;
    currentBoard[to.first - 1 - 'a'][to.second - 1] = rook;

    currentBoard[from.first - 'a'][from.second - 1] = new NullPiece{'*', '*'};
    currentBoard['a' - 'a'][7] = new NullPiece{'*', '*'};
    // set rook and king as moved
    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;

  } else if (fromPiece->getColor() == 'w' && to.first == 'g' &&
             to.second == 1 && fromPiece->getName() == 'K' &&
             getPieceAtPosition(std::make_pair('h', 1))->getName() == 'R') {
    Piece *rook = getPieceAtPosition(std::make_pair('h', 1));
    delete toPiece;

    currentBoard[to.first - 'a'][to.second - 1] = fromPiece;
    currentBoard[to.first - 'a' - 1][to.second - 1] = rook;

    currentBoard[from.first - 'a'][from.second - 1] = new NullPiece{'*', '*'};
    currentBoard['h' - 'a'][1 - 1] = new NullPiece{'*', '*'};
    // set rook and king as moved
    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();

    return;
  } else if (fromPiece->getColor() == 'w' && to == std::make_pair('c', 1) &&
             fromPiece->getName() == 'K' &&
             getPieceAtPosition(std::make_pair('a', 1))->getName() == 'R') {
    Piece *rook = getPieceAtPosition(std::make_pair('a', 1));
    delete toPiece;

    currentBoard[to.first - 'a'][to.second - 1] = fromPiece;
    currentBoard[to.first + 1 - 'a'][to.second - 1] = rook;

    currentBoard[from.first - 'a'][from.second - 1] = new NullPiece{'*', '*'};
    currentBoard['a' - 'a'][0] = new NullPiece{'*', '*'};
    // set rook and king as moved
    fromPiece->setPieceAsMoved();
    rook->setPieceAsMoved();
    return;
  }

  // EnPassant
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
        std::cout << "ENPASSANT" << std::endl;
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
        std::cout << "ENPASSANT" << std::endl;
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

void Board::setEnPassantPawn(std::pair<char, int> pawn) {
  this->enPassantPawn.first = pawn.first;
  this->enPassantPawn.second = pawn.second;
}

void Board::setPlayerTurn(AbstractPlayer *player) {
  this->whosPlayerTurn = player;
}

void Board::setWhitePlayer(AbstractPlayer *player) {
  this->whitePlayer = player;
}

void Board::setBlackPlayer(AbstractPlayer *player) {
  this->blackPlayer = player;
}

AbstractPlayer *Board::getWhitePlayer() { return this->whitePlayer; }

AbstractPlayer *Board::getBlackPlayer() { return this->blackPlayer; }

AbstractPlayer *Board::getWhosPlayerTurn() { return this->whosPlayerTurn; }

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

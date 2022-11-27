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

/* Board class */
Board::Board() {
  // start with board of nullpieces
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      this->currentBoard[x][y] = new NullPiece('*', '*');
    }
  }

  this->whosTurn = 'w';
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
    this->currentBoard[i][1] = new Pawn('P', 'w', true);
  }

  // Initialize black pieces
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
    this->currentBoard[i][6] = new Pawn('p', 'b', true);
  }

  generateCompleteMoves();
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

      newBoard->currentBoard[x][y] = this->currentBoard[x][y]->clone();

      // set BlackKing and WhiteKing fields.
      if (newBoard->currentBoard[x][y]->getName() == 'k') {
        newBoard->blackKing = newBoard->currentBoard[x][y];
      } else if (newBoard->currentBoard[x][y]->getName() == 'K') {
        newBoard->whiteKing = newBoard->currentBoard[x][y];
      }
    }
  }

  newBoard->whosTurn = this->whosTurn;
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

void Board::setTurn(char player) {
  if (player == 'w' || player == 'b') {
    this->whosTurn = player;
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

void Board::parsePossibleMovesKnight(Piece &knight,
                                     std::pair<char, int> position) {
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
    // diagonal moves have a diff. x coordinate and a diff. y coordinate
    if ((move.first != position.first) && (move.second != position.second)) {
      // if the square is not empty and it is the opponents piece, its a valid
      // move.

      // Here we will also need to check if moving the pawn causes check to its
      // own king.
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

  /* this is not ideal, we should have Piece.allPossibleMoves is a pointer to
    the vector, and tmp is a pointer to a vector. Then we can just swap the
    memory of the two vectors for optimal performance. */
  pawn.allPossibleMoves = tmp;
}

void Board::parsePossibleMovesKing(Piece &king, std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : king.allPossibleMoves) {
    // if the piece at 'move' is not the same color (or is empty)...
    if (this->getPieceAtPosition(move)->getColor() != king.getColor()) {
      /* we take the following steps to determine whether or not the King
          would be in check by taking a move */

      
      // create a temporary board
      Board *tmpBoard = this->clone();

      /*
      // move the King to this potential position
      tmpBoard->movePiece(position, move);

      if (king.getColor() == 'b') {
        if (inCheck(*(tmpBoard->getBlackKing()),
                    tmpBoard->getBlackKingPosition()) == false) {
          tmp.push_back(move);
        }
      } else if (king.getColor() == 'w') {
        if (inCheck(*(tmpBoard->getWhiteKing()),
                    tmpBoard->getWhiteKingPosition()) == false) {
          tmp.push_back(move);
        }
      }
      */

      delete tmpBoard;

      tmp.push_back(move);
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
      this->currentBoard[x][y]->getAllPossibleMoves(position);
      this->parsePossibleMoves(*(this->currentBoard[x][y]), position);
    }
  }
}

bool Board::inCheck(Piece &king, std::pair<char, int> currentPosition) {
  // this->generateCompleteMoves();

  if ((king.getName() != 'k') && (king.getName() != 'K')) {
    return false;
  } else {
    /*
    // get list of moves for King last
    if (king.getName() == 'k') {
      this->blackKing->getAllPossibleMoves(this->getBlackKingPosition());
      this->parsePossibleMoves(*(this->blackKing), this->getBlackKingPosition());
    } else {
      this->whiteKing->getAllPossibleMoves(this->getWhiteKingPosition());
      this->parsePossibleMoves(*(this->whiteKing), this->getWhiteKingPosition());
    }
    */

    // O(n^3) efficiency 💀 let's think of some optimization later.
    for (int x = 0; x < 8; x++) {
      for (int y = 0; y < 8; y++) {
        if (this->currentBoard[x][y]->getColor() != king.getColor() &&
            this->currentBoard[x][y]->getColor() != '*' &&
            this->currentBoard[x][y] != &king) {
          for (auto move : this->currentBoard[x][y]->allPossibleMoves) {
            if (move == currentPosition) {
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

void Board::movePiece(std::pair<char, int> oldPosition,
                      std::pair<char, int> newPosition) {
  // doing no checks right now, so there could be memory issues.
  int oldX = convertAlphaToNum(oldPosition.first);
  int oldY = oldPosition.second - 1;

  int newX = convertAlphaToNum(newPosition.first);
  int newY = newPosition.second - 1;

  Piece *oldPiece = this->getPieceAtPosition(oldPosition);
  Piece *newPiece = this->getPieceAtPosition(newPosition);

  // if the Piece to move isn't an empty square...
  if (oldPiece->getName() != '*') {

    // if the new position is in valid moves...
    for (auto move : oldPiece->allPossibleMoves) {
      if (move == newPosition) {
        // and if the square to move is not empty and not of the same colour...
        if (newPiece->getName() != '*' &&
            newPiece->getName() != oldPiece->getName()) {

          // capture the piece.
          delete newPiece;

          // and replace with an empty square.
          this->setPieceAtPosition(newPosition, this->createPiece('*'));
        }

        // Swap the empty square with the current Piece.
        // We need the coordinates, cause it doesn't work if we just swap
        //  oldPiece and newPiece.
        std::swap(this->currentBoard[oldX][oldY],
                  this->currentBoard[newX][newY]);

        if (this->currentBoard[newX][newY]->getName() == 'k') {
          this->setBlackKingPosition(newPosition);
        } else if (this->currentBoard[newX][newY]->getName() == 'K') {
          this->setWhiteKingPosition(newPosition);
        }

        // Here we should set inStartingPosition to false.
        // We should have a setter for startingPosition in the Piece class.

        generateCompleteMoves();

        return;
      }
    }
  }
}

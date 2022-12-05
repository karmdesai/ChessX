#include "board.h"

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

// Board constructor
Board::Board() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      this->currentBoard[x][y] = new NullPiece('*', '*');
    }
  }
  this->whosColourTurn = 'w';
}

// Board destructor
Board::~Board() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      delete this->currentBoard[x][y];
    }
  }
}

// if 'setup' is not called, we use the default board setup
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
int Board::convertAlphaToNum(char alpha) {
  if (alpha >= 'a' && alpha <= 'h') {
    return alpha - 'a';
  } else {
    return -1;
  }
}

// convertNumToAlpha() converts a number ranging from 0 to 7
//  to a character ranging from a to h.
char Board::convertNumToAlpha(int num) {
  if (num >= 0 && num <= 7) {
    return num + 'a';
  } else {
    return '*';
  }
}

// createPiece(c) takes a character c and creates a new Piece
//  of name c. It returns a pointer to the newly created piece.
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

// creates a deep copy of the current board and returns a pointer to it
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

// returns a pointer to the piece at the given coordinates
Piece *Board::getPieceAtPosition(std::pair<char, int> position) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  return this->currentBoard[x][y];
}

// returns a pointer to the white king on the board
Piece *Board::getWhiteKing() { return this->whiteKing; }

// returns a pointer to the black king on the board
Piece *Board::getBlackKing() { return this->blackKing; }

// returns the position of the white king on the board
std::pair<char, int> Board::getWhiteKingPosition() {
  return this->whiteKingPosition;
}

// returns the position of the black king on the board
std::pair<char, int> Board::getBlackKingPosition() {
  return this->blackKingPosition;
}

// sets the piece at a given position to a given piece
void Board::setPieceAtPosition(std::pair<char, int> position, Piece *p) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  y -= 1;

  this->currentBoard[x][y] = p;
}

// sets who's turn it is
void Board::setColourTurn(char player) {
  if (player == 'w' || player == 'b') {
    this->whosColourTurn = player;
  }
}

// sets the current white king for the board
void Board::setWhiteKing(Piece *wk) { this->whiteKing = wk; }

// sets the current black king for the board
void Board::setBlackKing(Piece *bk) { this->blackKing = bk; }

// sets the position of the white king on the board
void Board::setWhiteKingPosition(std::pair<char, int> position) {
  this->whiteKingPosition = position;
}

// sets the position of the black king on the board
void Board::setBlackKingPosition(std::pair<char, int> position) {
  this->blackKingPosition = position;
}

// get the colour of the piece at a given position
char Board::getColourTurn() { return this->whosColourTurn; }

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

// sets whether an en passant pawn is possible
void Board::setEnPassantPawn(std::pair<char, int> pawn) {
  this->enPassantPawn.first = pawn.first;
  this->enPassantPawn.second = pawn.second;
}

// sets the next player whos turn is next
void Board::setPlayerTurn(AbstractPlayer *player) {
  this->whosPlayerTurn = player;
}

// sets the white player for the current game
void Board::setWhitePlayer(AbstractPlayer *player) {
  this->whitePlayer = player;
}

// sets the black player for the current game
void Board::setBlackPlayer(AbstractPlayer *player) {
  this->blackPlayer = player;
}

// returns a pointer to the white player AbstractPlayer
AbstractPlayer *Board::getWhitePlayer() { return this->whitePlayer; }

// returns a pointer to the black player AbstractPlayer
AbstractPlayer *Board::getBlackPlayer() { return this->blackPlayer; }

// returns a pointer to the player whose turn it is
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

// helper functions for observers
bool Board::getEnPassantMade() { return enPassantMade; }
void Board::setEnPassantFalse() { enPassantMade = false; }

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
      delete currentBoard[x][y];
    }
  }
}

void Board::defaultInitialization() {
  // Initialize white pieces
  currentBoard[0][0] = new Rook('R', 'w', true);
  currentBoard[1][0] = new Knight('N', 'w');
  currentBoard[2][0] = new Bishop('B', 'w');
  currentBoard[3][0] = new Queen('Q', 'w');
  currentBoard[4][0] = new King('K', 'w', true);
  currentBoard[5][0] = new Bishop('B', 'w');
  currentBoard[6][0] = new Knight('N', 'w');
  currentBoard[7][0] = new Rook('R', 'w', true);

  for (int i = 0; i < 8; i++) {
    currentBoard[i][1] = new Pawn('P', 'w', true);
  }

  // Initialize black pieces
  currentBoard[0][7] = new Rook('r', 'b', true);
  currentBoard[1][7] = new Knight('n', 'b');
  currentBoard[2][7] = new Bishop('b', 'b');
  currentBoard[3][7] = new Queen('q', 'b');
  currentBoard[4][7] = new King('k', 'b', true);
  currentBoard[5][7] = new Bishop('b', 'b');
  currentBoard[6][7] = new Knight('n', 'b');
  currentBoard[7][7] = new Rook('r', 'b', true);

  for (int i = 0; i < 8; i++) {
    currentBoard[i][6] = new Pawn('p', 'b', true);
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
Piece* Board::createPiece(char c) {
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

void Board::parsePossibleMoves(Piece &piece, std::pair<char, int> position) {
  if (piece.getName() == 'p' || piece.getName() == 'P') {
    // Pawn
  } else if (piece.getName() == 'r' || piece.getName() == 'R') {
    // Rook
  } else if (piece.getName() == 'n' || piece.getName() == 'N') {
    parsePossibleMovesKnight(piece, position);
  } else if (piece.getName() == 'b' || piece.getName() == 'B') {
    // Bishop
  } else if (piece.getName() == 'q' || piece.getName() == 'Q') {
    // Queen
    parsePossibleMovesQueen(piece, position);
  } else if (piece.getName() == 'k' || piece.getName() == 'K') {
    // King
  }
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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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
        Piece *tmpPiece = pieceAtPosition(newMove);

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

bool Board::inCheck(Piece &king) {
  if ((king.getName() != 'k') && (king.getName() != 'K')) {
    return false;
  } else {
    // never in check for now.
    return false;
  }
}
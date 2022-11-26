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
Board::Board(bool customSetup) {
  // start with board of nullpieces
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      currentBoard[x][y] = new NullPiece(' ', ' ');
    }
  }

  if (customSetup == false) {
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
}

// Destructor
Board::~Board() {
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      delete currentBoard[x][y];
    }
  }
}

// Overload the << operator for the Board class
std::ostream &operator<<(std::ostream &out, const Board *myBoard) {
  for (int y = 7; y >= 0; y--) {
    out << y + 1 << " ";

    for (int x = 0; x < 8; x++) {
      Piece *currentSpace = myBoard->currentBoard[x][y];

      if (currentSpace != nullptr) {
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

Piece *Board::pieceAtPosition(std::pair<char, int> position) {
  int x = convertAlphaToNum(position.first);
  int y = position.second;

  /* Check if position is out of bounds first?

  if (x == -1 || y < 1 || y > 8) {
      return false;
  }
  */

  // input is always from 1 to 8 but array indexing is from 0 to 7.
  x -= 1;
  y -= 1;

  return currentBoard[x][y];
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
  
  // new list of possible moves
  std::vector<std::pair<char, int>> tmp;

  int x = convertAlphaToNum(position.first);
  int y = position.second;

  // check up moves, stop when piece is found
  for (int i = y + 1; i <= 8; ++i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(x, i));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(x, i));
    }
  }

  // check down moves, stop when piece is found
  for (int i = y - 1; i >= 1; --i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(x, i));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(x, i));
    }
  }

  // check left moves, stop when piece is found
  for (int i = x - 1; i >= 1; --i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(i, y));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(i, y));
    }
  }

  // check right moves, stop when piece is found
  for (int i = x + 1; i <= 8; ++i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(i, y));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(i, y));
    }
  }

  // check diagonally up and to the right
  for (int i = 1; i <= 8; ++i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(x + i, y + i));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(x + i, y + i));
    }
  }

  // check diagonally up and to the left
  for (int i = 1; i <= 8; ++i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(x - i, y + i));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(x - i, y + i));
    }
  }

  // check diagonally down and to the right
  for (int i = 1; i <= 8; ++i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(x + i, y - i));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(x + i, y - i));
    }
  }

  // check diagonally down and to the left
  for (int i = 1; i <= 8; ++i) {
    Piece *currentPiece = pieceAtPosition(std::make_pair(x - i, y - i));

    if (currentPiece->getName() != '*') {
      break;
    } else {
      tmp.push_back(std::make_pair(x - i, y - i));
    }
  }

  // update possible moves
  queen.allPossibleMoves = tmp;
}

void Board::parsePossibleMovesKnight(Piece &knight,
                                     std::pair<char, int> position) {
  std::vector<std::pair<char, int>> tmp;

  for (auto move : knight.allPossibleMoves) {
    // as long as the piece at 'move' is not the same color, its a valid move
    if (this->pieceAtPosition(move)->getColor() != knight.getColor()) {
      tmp.push_back(move);
    }
  }
  /* this is not ideal, we should have Piece.allPossibleMoves is a pointer to
    the vector, and tmp is a pointer to a vector. Then we can just swap the
    memory of the two vectors for optimal performance. */
  knight.allPossibleMoves = tmp;
}

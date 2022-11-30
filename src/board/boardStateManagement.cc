#include "../pieces/nullPiece.h"
#include "board.h"
#include <iostream>

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

void Board::movePiece(std::pair<char, int> from, std::pair<char, int> to) {
  Piece *currentPiece = this->getPieceAtPosition(from);

  for (auto move : currentPiece->allPossibleMoves) {
    if (move == to) {
      Board *tmpBoard = this->clone();

      tmpBoard->movePieceBase(from, to);

      if (currentPiece->getColor() == 'b') {
        if (tmpBoard->inCheck(*(tmpBoard->getBlackKing()),
                              tmpBoard->getBlackKingPosition()) == false) {
          this->movePieceBase(from, to);
        } else {
          std::cout << "Illegal move! That would put the Black King in check."
                    << std::endl;
        }
      } else if (currentPiece->getColor() == 'w') {
        if (tmpBoard->inCheck(*(tmpBoard->getWhiteKing()),
                              tmpBoard->getWhiteKingPosition()) == false) {
          this->movePieceBase(from, to);
        } else {
          std::cout << "Illegal move! That would put the White King in check."
                    << std::endl;
        }
      }

      delete tmpBoard;

      return;
    }
  }
}

void Board::movePieceBase(std::pair<char, int> from, std::pair<char, int> to) {
  Piece *fromPiece = getPieceAtPosition(from);
  Piece *toPiece = getPieceAtPosition(to);

  if (fromPiece->getName() != '*' &&
      (fromPiece->getColor() != toPiece->getColor())) {

    // if the piece is NullPiece or opponent piece...
    delete toPiece;

    // move the piece
    currentBoard[to.first - 'a'][to.second - 1] = fromPiece;

    if (this->currentBoard[to.first - 'a'][to.second - 1]->getName() == 'k') {
      this->setBlackKingPosition(to);
    } else if (currentBoard[to.first - 'a'][to.second - 1]->getName() == 'K') {
      this->setWhiteKingPosition(to);
    }

    // set old position to a new null piece
    currentBoard[from.first - 'a'][from.second - 1] = new NullPiece{'*', '*'};
  }
}

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
            if (this->currentBoard[x][y]->getName() == 'p') {
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

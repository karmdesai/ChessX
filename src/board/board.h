#ifndef BOARD_H
#define BOARD_H

#include <ostream>

#include "../pieces/piece.h"

class Board {
  private:
    // We have an 8x8 x-y coordinate system.
    // The first index is the x-coordinate (which ranges from 0 to 7).
    //  The first index represents the alphabetical column of the board (a-h).
    // The second index is the y-coordinate (which ranges from 0 to 7).
    Piece *currentBoard[8][8];

    char whosTurn;

    // We keep track of this because we are going to be calling
    //  inCheck atleast once every turn. So instead of calling a nested
    //  for loop every time, we just deal with tracking this manually instead.
    Piece *whiteKing;
    Piece *blackKing;

    std::pair<char, int> whiteKingPosition;
    std::pair<char, int> blackKingPosition;

  public:
    Board();
    ~Board();

    void defaultInitialization();

    // To deep copy the board
    Board *clone();

    // Overloaded operator<< to print board
    friend std::ostream &operator<<(std::ostream &out, const Board *myBoard);

    // Helpers
    int convertAlphaToNum(char alpha);
    char convertNumToAlpha(int num);

    Piece *createPiece(char);

    // Getters
    Piece *getPieceAtPosition(std::pair<char, int> position);

    Piece *getWhiteKing();
    Piece *getBlackKing();

    std::pair<char, int> getWhiteKingPosition();
    std::pair<char, int> getBlackKingPosition();

    // Setters
    void setPieceAtPosition(std::pair<char, int> position, Piece *p);
    void setTurn(char player);

    void setWhiteKing(Piece *wk);
    void setBlackKing(Piece *bk);

    void setWhiteKingPosition(std::pair<char, int> position);
    void setBlackKingPosition(std::pair<char, int> position);

    // Move parsers/validators
    void parsePossibleMoves(Piece &piece, std::pair<char, int> position);
    void parsePossibleMovesKing(Piece &king, std::pair<char, int> position);
    void parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position);
    void parsePossibleMovesQueen(Piece &queen, std::pair<char, int> position);
    void parsePossibleMovesRook(Piece &rook, std::pair<char, int> position);
    void parsePossibleMovesBishop(Piece &bishop, std::pair<char, int> position);
    void parsePossibleMovesPawn(Piece &pawn, std::pair<char, int> position);

    void generateCompleteMoves();

    // Condition managers (check, checkmate, draw, win, etc.)
    bool inCheck(Piece &king, std::pair<char, int> currentPosition);
    void movePiece(std::pair<char, int> oldPosition,
                   std::pair<char, int> newPosition);
    void movePieceBase(std::pair<char, int> oldPosition,
                          std::pair<char, int> newPosition);
};

#endif

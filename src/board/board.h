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

    // We keep track of this because we are going to be calling
    //  inCheck atleast once every turn. So instead of calling a nested
    //  for loop every time, we just deal with tracking this manually instead.
    Piece *whiteKing;
    Piece *blackKing;
    std::pair<char, int> whiteKingPosition;
    std::pair<char, int> blackKingPosition;

    char whosTurn;

  public:
    /* Board Creation and Destruction Methods (boardCreationDestruction.cc) */
    Board();
    ~Board();

    void defaultInitialization();
    Board *clone();

    /* Board Utility and Helper Methods (boardUtility.cc) */
    friend std::ostream &operator<<(std::ostream &out, const Board *myBoard);

    int convertAlphaToNum(char alpha);
    char convertNumToAlpha(int num);

    Piece *createPiece(char);

    /* Board Getters and Setters (boardGettersAndSetters.cc) */
    Piece *getPieceAtPosition(std::pair<char, int> position);
    void setPieceAtPosition(std::pair<char, int> position, Piece *p);

    char getTurn();
    void setTurn(char player);

    Piece *getWhiteKing();
    void setWhiteKing(Piece *wk);
    Piece *getBlackKing();
    void setBlackKing(Piece *bk);

    std::pair<char, int> getWhiteKingPosition();
    void setWhiteKingPosition(std::pair<char, int> position);
    std::pair<char, int> getBlackKingPosition();
    void setBlackKingPosition(std::pair<char, int> position);

    /* Board Move Parsers (boardParsers.cc) */
    void parsePossibleMoves(Piece &piece, std::pair<char, int> position);
    void parsePossibleMovesKing(Piece &king, std::pair<char, int> position);
    void parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position);
    void parsePossibleMovesPawn(Piece &pawn, std::pair<char, int> position);
    void parsePossibleMovesQueen(Piece &queen, std::pair<char, int> position);
    void parsePossibleMovesRook(Piece &rook, std::pair<char, int> position);
    void parsePossibleMovesBishop(Piece &bishop, std::pair<char, int> position);

    // Board State Checkers and Modifiers (boardStateManagement.cc) */
    bool inCheck(Piece &king, std::pair<char, int> currentPosition);

    void movePiece(std::pair<char, int> oldPosition,
                   std::pair<char, int> newPosition);
    void movePieceBase(std::pair<char, int> oldPosition,
                          std::pair<char, int> newPosition);

    void generateCompleteMoves();
    std::vector<std::pair<char, int>> generateThreatMap(Piece *p);
};

#endif

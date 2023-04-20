#ifndef BOARD_H
#define BOARD_H

#include <ostream>

#include "../computer/abstractPlayer.h"
#include "../pieces/piece.h"

class Board {
 private:
  // We have an 8x8 x-y coordinate system.
  // The first index is the x-coordinate (which ranges from 0 to 7).
  //  The first index represents the alphabetical column of the board (a-h).
  // The second index is the y-coordinate (which ranges from 0 to 7).
  Piece *currentBoard[8][8];

  // keep track of current player and color
  char whosColourTurn;
  AbstractPlayer *whosPlayerTurn;

  Piece *whiteKing;
  Piece *blackKing;

  std::pair<char, int> whiteKingPosition;
  std::pair<char, int> blackKingPosition;
  std::pair<char, int> enPassantPawn;
  bool enPassantValid = false;
  bool enPassantMade = false;

  // keep track of players
  AbstractPlayer *whitePlayer;
  AbstractPlayer *blackPlayer;

 public:
  /* Board Creation and Destruction Methods (boardCreationDestruction.cc) */
  Board();
  ~Board();

  void defaultInitialization();
  // To deep copy the board
  Board *clone();

  /* Board Utility and Helper Methods (boardUtility.cc) */
  // Overloaded operator<< to print board (debugging)
  friend std::ostream &operator<<(std::ostream &out, const Board *myBoard);

  // general-use helpers
  int convertAlphaToNum(char alpha);
  char convertNumToAlpha(int num);
  Piece *createPiece(char);

  /* Board Getters and Setters (boardGettersAndSetters.cc) */
  // general-use getters
  Piece *getPieceAtPosition(std::pair<char, int> position);
  Piece *getWhiteKing();
  Piece *getBlackKing();
  std::pair<char, int> getWhiteKingPosition();
  std::pair<char, int> getBlackKingPosition();
  AbstractPlayer *getWhitePlayer();
  AbstractPlayer *getBlackPlayer();
  AbstractPlayer *getWhosPlayerTurn();
  char getColourTurn();

  // general use setters
  void setPieceAtPosition(std::pair<char, int> position, Piece *p);
  void setColourTurn(char player);
  void setPlayerTurn(AbstractPlayer *player);
  void setWhiteKing(Piece *wk);
  void setBlackKing(Piece *bk);
  void setWhiteKingPosition(std::pair<char, int> position);
  void setBlackKingPosition(std::pair<char, int> position);
  void setWhitePlayer(AbstractPlayer *wp);
  void setBlackPlayer(AbstractPlayer *bp);

  /* Board Move Parsers (boardParsers.cc) */
  void generateCompleteMoves();
  std::vector<std::pair<char, int>> generateThreatMap(Piece *p);
  
  void parsePossibleMoves(Piece &piece, std::pair<char, int> position);
  void parsePossibleMovesKing(Piece &king, std::pair<char, int> position);
  void parsePossibleMovesKnight(Piece &knight, std::pair<char, int> position);
  void parsePossibleMovesQueen(Piece &queen, std::pair<char, int> position);
  void parsePossibleMovesRook(Piece &rook, std::pair<char, int> position);
  void parsePossibleMovesBishop(Piece &bishop, std::pair<char, int> position);
  void parsePossibleMovesPawn(Piece &pawn, std::pair<char, int> position);

  // Board State Checkers and Modifiers (boardStateManagement.cc) */
  bool isPieceCapturable(Piece *piece, std::pair<char, int> position);
  bool inCheck(Piece &king, std::pair<char, int> currentPosition);
  bool isInsufficientMaterial();
  bool movePiece(std::pair<char, int> oldPosition,
                 std::pair<char, int> newPosition);
  bool movePieceBase(std::pair<char, int> oldPosition,
                     std::pair<char, int> newPosition, char promote);
  void movePieceBase(std::pair<char, int> oldPosition,
                     std::pair<char, int> newPosition);

  // En Passant Helpers (boardEnPassant.cc) */
  void setEnPassantPawn(std::pair<char, int> pawn);
  bool getEnPassantMade();
  void setEnPassantFalse();
};

#endif

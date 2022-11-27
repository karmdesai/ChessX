#ifndef ABSTRACT_PLAYER_H
#define ABSTRACT_PLAYER_H

class Board;

class AbstractPlayer {
 protected:
  char playerColor;
  Board *board;

 public:
  AbstractPlayer(char playerColor, Board *board);
  virtual ~AbstractPlayer() = default;

  // Getters
  char getPlayerColor();
  Board *getBoard();
};

#endif

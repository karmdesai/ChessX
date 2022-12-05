#ifndef ABSTRACT_PLAYER_H
#define ABSTRACT_PLAYER_H

#include <vector>

class Board;

class AbstractPlayer {
 protected:
  char playerColor;
  Board *board;
  bool isAComputer;

  // markers to see when computer can't make any more moves
  std::pair<std::pair<char, int>, std::pair<char, int>> CHECKMATE =
      std::make_pair(std::make_pair('a', -1), std::make_pair('a', -1));
  std::pair<std::pair<char, int>, std::pair<char, int>> STALEMATE =
      std::make_pair(std::make_pair('a', 0), std::make_pair('a', 0));

 public:
  AbstractPlayer(char playerColor, Board *board, bool isAComputer);
  virtual ~AbstractPlayer() = default;

  // Getters
  char getPlayerColor();
  Board *getBoard();
  bool isComputer();

  // calculate the next move, returns a pair of moves
  virtual std::pair<std::pair<char, int>, std::pair<char, int>>
  calculateNextMove() = 0;

  // random number generator
  int randomNumber(int x, int y);
};

#endif

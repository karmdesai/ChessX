#include "GUIObs.h"

#include <iostream>

#include "observer.h"
#include "studio.h"
#include "window.h"

GraphObs::GraphObs(Studio *canvas) : canvas{canvas} { canvas->attach(this); }

GraphObs::~GraphObs() {
  canvas->detach(this);
  delete w;
}

Xwindow *GraphObs::getWindowAdd() { return w; }

void drawKing(Xwindow *w, char col, int startX, int startY) {
  int color = 0;
  if (col == 'b') color = 1;

  int bgcol = 4;
  if ((startX + startY) % 2 != 0) bgcol = 2;

  w->fillRectangle(startX * 80, startY * 80, 80, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 10, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 10, 10, bgcol);
  w->fillRectangle(startX * 80 + 40, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 30, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 20, 10, color);
  w->fillRectangle(startX * 80 + 40, startY * 80 + 30, 40, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 40, 10, 10, bgcol);
  w->fillRectangle(startX * 80 + 40, startY * 80 + 40, 10, 10, color);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 40, 30, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 50, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 60, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, bgcol);
}

void drawPawn(Xwindow *w, char col, int startX, int startY) {
  int color = 0;
  if (col == 'b') color = 1;

  int bgcol = 4;
  if ((startX + startY) % 2 != 0) bgcol = 2;

  w->fillRectangle(startX * 80, startY * 80, 80, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 40, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 50, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 50, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, bgcol);
}

void drawBishop(Xwindow *w, char col, int startX, int startY) {
  int color = 0;
  if (col == 'b') color = 1;

  int bgcol = 4;
  if ((startX + startY) % 2 != 0) bgcol = 2;

  w->fillRectangle(startX * 80, startY * 80, 80, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 30, 10, color);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 10, 30, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 30, 10, color);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 40, 30, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 50, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 30, 10, color);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 60, 30, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, bgcol);
}

void drawKnight(Xwindow *w, char col, int startX, int startY) {
  int color = 0;
  if (col == 'b') color = 1;

  int bgcol = 4;
  if ((startX + startY) % 2 != 0) bgcol = 2;

  w->fillRectangle(startX * 80, startY * 80, 80, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 10, 50, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 40, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 40, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 50, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 60, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, bgcol);
}

void drawRook(Xwindow *w, char col, int startX, int startY) {
  int color = 0;
  if (col == 'b') color = 1;

  int bgcol = 4;
  if ((startX + startY) % 2 != 0) bgcol = 2;

  w->fillRectangle(startX * 80, startY * 80, 80, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 30, 10, color);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 40, 30, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 50, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 50, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 60, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, bgcol);
}

void drawQueen(Xwindow *w, char col, int startX, int startY) {
  int color = 0;
  if (col == 'b') color = 1;

  int bgcol = 4;
  if ((startX + startY) % 2 != 0) bgcol = 2;

  w->fillRectangle(startX * 80, startY * 80, 80, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 10, 10, color);
  w->fillRectangle(startX * 80 + 30, startY * 80 + 40, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 40, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, bgcol);
  w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 40, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 50, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 60, 50, 10, bgcol);
  w->fillRectangle(startX * 80 + 50, startY * 80 + 60, 10, 10, color);
  w->fillRectangle(startX * 80 + 60, startY * 80 + 60, 20, 10, bgcol);

  w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, bgcol);
}

void drawBlank(Xwindow *w, int startX, int startY) {
  if ((startX + startY) % 2 == 0) {
    w->fillRectangle(startX * 80, startY * 80, 80, 80, Xwindow::Peru);
  } else {
    w->fillRectangle(startX * 80, startY * 80, 80, 80, Xwindow::Sienna);
  }
}

void GraphObs::notify(std::pair<char, int> from, std::pair<char, int> to,
                      bool enPassant) {
  if (from.first == 'o') {
    for (int y = 8; y > 0; y--) {
      for (int x = 0; x < 8; x++) {
        Piece *currentSpace = canvas->getState(std::make_pair(x + 'a', y));
        if (currentSpace->getName() != '*') {
          switch (currentSpace->getName()) {
            case 'r':
              drawRook(getWindowAdd(), 'b', x, 8 - y);
              break;
            case 'n':
              drawKnight(getWindowAdd(), 'b', x, 8 - y);
              break;
            case 'b':
              drawBishop(getWindowAdd(), 'b', x, 8 - y);
              break;
            case 'q':
              drawQueen(getWindowAdd(), 'b', x, 8 - y);
              break;
            case 'k':
              drawKing(getWindowAdd(), 'b', x, 8 - y);
              break;
            case 'p':
              drawPawn(getWindowAdd(), 'b', x, 8 - y);
              break;
            case 'R':
              drawRook(getWindowAdd(), 'w', x, 8 - y);
              break;
            case 'N':
              drawKnight(getWindowAdd(), 'w', x, 8 - y);
              break;
            case 'B':
              drawBishop(getWindowAdd(), 'w', x, 8 - y);
              break;
            case 'Q':
              drawQueen(getWindowAdd(), 'w', x, 8 - y);
              break;
            case 'K':
              drawKing(getWindowAdd(), 'w', x, 8 - y);
              break;
            case 'P':
              drawPawn(getWindowAdd(), 'w', x, 8 - y);
              break;
            default:
              break;
          }
        } else {
          drawBlank(getWindowAdd(), x, 8 - y);
        }
      }
    }
  } else if (enPassant) {
    Piece *toPiece = canvas->getState(to);
    std::pair<char, int> captureCord;
    if (toPiece->getColor() == 'w') {
      captureCord = std::make_pair(to.first, to.second - 1);
    } else if (toPiece->getColor() == 'b') {
      captureCord = std::make_pair(to.first, to.second + 1);
    }

    drawBlank(getWindowAdd(), captureCord.first - 'a', 8 - captureCord.second);
    drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
    drawPawn(getWindowAdd(), toPiece->getColor(), to.first - 'a',
             8 - to.second);
    return;
  } else {
    // Piece *fromPiece = canvas->getState(from);
    Piece *toPiece = canvas->getState(to);

    if (toPiece->getColor() == 'b' && to == std::make_pair('g', 8) &&
        toPiece->getName() == 'k' &&
        canvas->getState(std::make_pair('f', 8))->getName() == 'r') {
      drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
      drawBlank(getWindowAdd(), 'h' - 'a', 8 - from.second);
      drawKing(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      drawRook(getWindowAdd(), 'b', to.first - 'a' - 1, 8 - to.second);
      return;
    } else if (toPiece->getColor() == 'b' && to == std::make_pair('c', 8) &&
               toPiece->getName() == 'k' &&
               canvas->getState(std::make_pair('d', 8))->getName() == 'r') {
      drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
      drawBlank(getWindowAdd(), 'a' - 'a', 8 - from.second);
      drawKing(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      drawRook(getWindowAdd(), 'b', to.first - 'a' + 1, 8 - to.second);
      return;

    } else if (toPiece->getColor() == 'w' && to.first == 'g' &&
               to.second == 1 && toPiece->getName() == 'K' &&
               canvas->getState(std::make_pair('f', 1))->getName() == 'R') {
      drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
      drawBlank(getWindowAdd(), 'h' - 'a', 8 - from.second);
      drawKing(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      drawRook(getWindowAdd(), 'w', to.first - 'a' - 1, 8 - to.second);
      return;
    } else if (toPiece->getColor() == 'w' && to == std::make_pair('c', 1) &&
               toPiece->getName() == 'K' &&
               canvas->getState(std::make_pair('d', 1))->getName() == 'R') {
      drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
      drawBlank(getWindowAdd(), 'a' - 'a', 8 - from.second);
      drawKing(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      drawRook(getWindowAdd(), 'w', to.first - 'a' + 1, 8 - to.second);
      return;
    }

    drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
    switch (canvas->getState(to)->getName()) {
      case 'r':
        drawRook(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
        break;
      case 'n':
        drawKnight(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
        break;
      case 'b':
        drawBishop(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
        break;
      case 'q':
        drawQueen(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
        break;
      case 'k':
        drawKing(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
        break;
      case 'p':
        drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
        break;
      case 'R':
        drawRook(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
        break;
      case 'N':
        drawKnight(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
        break;
      case 'B':
        drawBishop(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
        break;
      case 'Q':
        drawQueen(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
        break;
      case 'K':
        drawKing(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
        break;
      case 'P':
        drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
        break;
      default:
        break;
    }
  }
}

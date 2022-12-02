#include "observer.h"
#include "studio.h"
#include "GUIObs.h"
#include <iostream>
#include "window.h"
using namespace std;

GraphObs::GraphObs(Studio *canvas): canvas{canvas} {
    canvas->attach(this);
}

GraphObs::~GraphObs() {
  canvas->detach(this);
}

Xwindow* GraphObs::getWindowAdd() {
  return w;
}

void drawKing(Xwindow *w, char col, int startX, int startY) {

}

void drawPawn(Xwindow *w, char col, int startX, int startY) {
  if (col == 'b') {
    if ((startX + startY) % 2 == 0) {
      w->fillRectangle(startX * 80, startY * 80, 80, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 40, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 50, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 50, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, Xwindow::Blue);

    } else {
      w->fillRectangle(startX * 80, startY * 80, 80, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 40, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 50, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, Xwindow::Black);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 50, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, Xwindow::Red);
    }
  } else {
    if ((startX + startY) % 2 == 0) {
      w->fillRectangle(startX * 80, startY * 80, 80, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 40, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 50, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, Xwindow::Blue);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 50, 10, Xwindow::Blue);

      w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, Xwindow::Blue);

    } else {
      w->fillRectangle(startX * 80, startY * 80, 80, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 10, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 10, 40, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 10, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 20, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 20, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 20, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 20, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 20, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 30, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 30, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 30, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 50, startY * 80 + 30, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 30, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 40, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 40, 40, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 60, startY * 80 + 40, 20, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 50, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 50, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 50, 50, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 60, 20, 10, Xwindow::Red);
      w->fillRectangle(startX * 80 + 20, startY * 80 + 60, 10, 10, Xwindow::White);
      w->fillRectangle(startX * 80 + 30, startY * 80 + 60, 50, 10, Xwindow::Red);

      w->fillRectangle(startX * 80, startY * 80 + 70, 80, 10, Xwindow::Red);
    }
  }
}

void drawBishop(Xwindow *w, char col, int startX, int startY) {

}

void drawKnight(Xwindow *w, char col, int startX, int startY) {

} 

void drawRook(Xwindow *w, char col, int startX, int startY) {

}

void drawQueen(Xwindow *w, char col, int startX, int startY) {

}

void drawBlank(Xwindow *w, int startX, int startY) {
  if ((startX + startY) % 2 == 0) {
    w->fillRectangle(startX * 80, startY * 80, 80, 80, Xwindow::Blue);
  } else {
    w->fillRectangle(startX * 80, startY * 80, 80, 80, Xwindow::Red);
  }
}

void GraphObs::notify(std::pair<char, int> from, std::pair<char, int> to) {
  if (from.first == 'o') {
    for (int y = 1; y <= 8; y++) {
      for (int x = 0; x < 8; x++) {
        Piece *currentSpace = canvas->getState(make_pair(x + 'a', y));
        if (currentSpace->getName() != '*') {
          switch (currentSpace->getName())
          {
          case 'r':
            drawPawn(getWindowAdd(), 'b', x, 8 - y);
            break;
          case 'n':
            drawPawn(getWindowAdd(), 'b', x, 8 - y);
            break;
          case 'b':
            drawPawn(getWindowAdd(), 'b', x, 8- y);
            break;
          case 'q':
            drawPawn(getWindowAdd(), 'b', x, 8- y);
            break;
          case 'k':
            drawPawn(getWindowAdd(), 'b', x, 8- y);
            break;
          case 'p':
            drawPawn(getWindowAdd(), 'b', x, 8- y);
            break;
          case 'R':
            drawPawn(getWindowAdd(), 'w', x, 8- y);
            break;
          case 'N':
            drawPawn(getWindowAdd(), 'w', x, 8- y);
            break;
          case 'B':
            drawPawn(getWindowAdd(), 'w', x, 8- y);
            break;
          case 'Q':
            drawPawn(getWindowAdd(), 'w', x, 8- y);
            break;
          case 'K':
            cout << "LFMAOFAMOFMAOPFASOP";
            drawPawn(getWindowAdd(), 'w', x, 8- y);
            break;
          case 'P':
            drawPawn(getWindowAdd(), 'w', x, 8- y);
            break;
          default:
            break;
          }
        } else {
          drawBlank(w, x, y - 1);
        }
      }
    }
  } else {
    drawBlank(getWindowAdd(), from.first - 'a', 8 - from.second);
    switch (canvas->getState(to)->getName())
    {
    case 'r':
      drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      break;
    case 'n':
      drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      break;
    case 'b':
      drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      break;
    case 'q':
      drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      break;
    case 'k':
      drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      break;
    case 'p':
      drawPawn(getWindowAdd(), 'b', to.first - 'a', 8 - to.second);
      break;
    case 'R':
      drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      break;
    case 'N':
      drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      break;
    case 'B':
      drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      break;
    case 'Q':
      drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      break;
    case 'K':
      drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      break;
    case 'P':
      drawPawn(getWindowAdd(), 'w', to.first - 'a', 8 - to.second);
      break;
    default:
      break;
    }
  }
}

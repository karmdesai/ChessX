#ifndef GUI_H
#define GUI_H
#include "observer.h"
#include "studio.h"
#include "window.h"

class GraphObs: public Observer {

  Studio *canvas;
  Xwindow *w = new Xwindow{640, 640};
  public:
    GraphObs(Studio *canvas);
    void notify(std::pair<char, int> from, std::pair<char, int> to, bool enPassant) override;
    ~GraphObs();
    Xwindow* getWindowAdd();
};

#endif

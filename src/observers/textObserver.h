#ifndef TEXTOBSERVER_H
#define TEXTOBSERVER_H

#include <algorithm>
#include "observer.h"
#include "studio.h"

class TextObs: public Observer {

  Studio *canvas;

  public:
    TextObs(Studio *canvas);
    void notify(std::pair<char, int> from, std::pair<char, int> to) override;
    ~TextObs();
};

#endif
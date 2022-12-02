#ifndef TEXTOBSERVER_H
#define TEXTOBSERVER_H

#include <algorithm>
#include "observer.h"
#include "studio.h"

class TextObs: public Observer {

  Studio *canvas;

  public:
    TextObs(Studio *canvas);
    void notify() override;
    ~TextObs();
};

#endif
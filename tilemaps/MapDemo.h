#ifndef MAPDEMO_H
#define MAPDEMO_H

#include "Game.h"
#include "Map.h"

class MapDemo : public Game
{
private:
    Map map;
    Image background;
public:
    MapDemo();
    ~MapDemo();
    virtual bool init();
    virtual void update();
    virtual void draw(Graphics* g);
    virtual void free();
};

#endif

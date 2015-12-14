#ifndef MAPDEMO_H
#define MAPDEMO_H

#include "Game.h"
#include "Image.h"
#include "Scene.h"
#include "Rectangle.h"
#include "Map.h"
#include "MapNode.h"
#include "PlayerNode.h"


#define SPRITE_DIM (32)
#define SCREEN_DIM (SPRITE_DIM * 50)


class SceneDemo : public Game
{
private:
    static const int PLAYER_SPEED = 10;

    Map map;
    Scene scene;
    Rectangle camera;
    MapNode* player;
public:
    SceneDemo();
    ~SceneDemo();
    virtual bool init();
    virtual void update();
    virtual void draw(Graphics* g);
    virtual void free();
};

#endif

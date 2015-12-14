#include "SceneDemo.h"

SceneDemo::SceneDemo()
{

}

SceneDemo::~SceneDemo()
{

}

bool SceneDemo::init()
{
    if(!initSystem("Maze", 32 * 29, 32 * 29, false))
        return false;

    if(!map.load("graphics/map.map", "graphics/tiles.bmp"))
        return false;

    player = new MapNode(0, 14 * SPRITE_DIM, 0 * SPRITE_DIM, SPRITE_DIM, SPRITE_DIM, true, &map);

    scene.addNode(player);

    camera.set(0,0,928,928);

    return true;
}

void SceneDemo::update()
{
    scene.update();

    Input* in = getInput();

    int playerMoveX = 0;
    int playerMoveY = 0;

    int oldX = player->getX();
    int oldY = player->getY();

    if(in->keyDown(SDLK_UP))
        playerMoveY = -PLAYER_SPEED;

    if(in->keyDown(SDLK_DOWN))
        playerMoveY = PLAYER_SPEED;

    if(in->keyDown(SDLK_LEFT))
        playerMoveX = -PLAYER_SPEED;

    if(in->keyDown(SDLK_RIGHT))
        playerMoveX = PLAYER_SPEED;

    player->move(playerMoveX, playerMoveY);

    camera.setCenterX(player->getCenterX());
    camera.setCenterY(player->getCenterY());

    if(camera.getX() < 0)
        camera.setX(0);

    if(camera.getY() < 0)
        camera.setY(0);

    if(camera.getX2() >= map.getTotalWidth())
        camera.setX2(map.getTotalWidth() - 1);

    if(camera.getY2() >= map.getTotalHeight())
        camera.setY2(map.getTotalHeight() - 1);

    if(player->getX() < 0)
        player->setX(0);

    if(player->getY() < 0)
        player->setY(0);

    if(player->getX2() >= map.getTotalWidth())
        player->setX2(map.getTotalWidth() - 1);

    if(player->getY2() >= map.getTotalHeight())
        player->setY2(map.getTotalHeight() - 1);

    // printf("%s\n", (map.checkSolid(player->getX(), player->getY())) ? "true" : "false");

    if(map.checkSolid(player->getX(), player->getY())){
        player->setX(oldX);
        player->setY(oldY);
    }


    if(player->getY()==896){
        player->fun();
    }
}

void SceneDemo::draw(Graphics* g)
{
    map.draw(1, camera.getX(), camera.getY(), g);
    map.draw(2, camera.getX(), camera.getY(), g);
    map.draw(0, camera.getX(), camera.getY(), g);
    scene.draw(&camera, g);
}

void SceneDemo::free()
{
    scene.removeNode(NULL);

    map.free();
    freeSystem();
}

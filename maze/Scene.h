#ifndef SCENE_H
#define SCENE_H

#include <list>

#include "SceneNode.h"
#include "Rectangle.h"

using namespace std;

class SceneNode;

class Scene
{
private:
    list<SceneNode*> nodes;
public:
    Scene();
    ~Scene();

    void addNode(SceneNode* node);
    void removeNode(SceneNode* node);

    void update();
    void draw(Rectangle* view, Graphics* g);

    list<SceneNode*>* getNodes();
};

#endif

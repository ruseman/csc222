#ifndef SCENENODE_H
#define SCENENODE_H

#include "Graphics.h"
#include "Scene.h"
#include "Rectangle.h"

class Scene;

class SceneNode : public Rectangle
{
private:
    Scene* scene;
    bool removed;
    int ID;
public:
    SceneNode(int i, int x, int y, int width, int height);
    ~SceneNode();

    void setScene(Scene* s);
    Scene* getScene();

    virtual void update();
    virtual void draw(Rectangle* view, Graphics* g);

    void remove();
    bool isRemoved();

    int getID();
};

#endif

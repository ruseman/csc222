#include "SceneNode.h"

SceneNode::SceneNode(int i, int x, int y, int width, int height) : Rectangle(x,y,width,height)
{
    ID = i;
    scene = NULL;
    removed = false;
}

SceneNode::~SceneNode()
{

}

void SceneNode::setScene(Scene* s)
{
    scene = s;
}

Scene* SceneNode::getScene()
{
    return scene;
}

void SceneNode::update()
{

}

void SceneNode::draw(Rectangle* view, Graphics* g)
{
    if(view->overlaps(*this) && g != NULL)
        g->drawRect(getX() - view->getX(), getY() - view->getY(), getWidth(), getHeight(), 255, 0, 0);
}

void SceneNode::remove()
{
    removed = true;
}

bool SceneNode::isRemoved()
{
    return removed;
}

int SceneNode::getID()
{
    return ID;
}


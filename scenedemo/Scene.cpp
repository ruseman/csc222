#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

void Scene::addNode(SceneNode* node)
{
    node->setScene(this);
    nodes.push_back(node);
}

void Scene::removeNode(SceneNode* node)
{
    if(node != NULL)
    {
        SceneNode* n = node;
        nodes.remove(node);
        delete n;
    }
    else
    {
        while(!nodes.empty())
        {
            removeNode(*nodes.begin());
        }
    }
}

void Scene::update()
{
    list<SceneNode*>::iterator it;

    //Remove 'removed' nodes
    for(it = nodes.begin(); it != nodes.end(); it++)
    {
        if((*it)->isRemoved())
        {
            SceneNode* oldNode = (*it);
            it--;
            removeNode(oldNode);
        }
    }

    //Update Nodes
    for(it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->update();
    }
}

void Scene::draw(Rectangle* view, Graphics* g)
{
    for(list<SceneNode*>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->draw(view, g);
    }
}

list<SceneNode*>* Scene::getNodes()
{
    return &nodes;
}


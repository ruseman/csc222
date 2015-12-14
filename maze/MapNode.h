#ifndef MAPNODE_H
#define MAPNODE_H

#include "Map.h"
#include "SceneNode.h"

class MapNode : public SceneNode
{
private:
    Map* map;
    bool solid;
    struct{
    	int r;
    	int g;
    	int b;
    }
    color;
    bool shuffle;
public:
    MapNode(int i, int x, int y, int width, int height, bool s, Map* m);
    ~MapNode();

    virtual void draw(Rectangle* view, Graphics* g);

    bool overlapsMap();

    void fun();

    void move(int x, int y);
};

#endif

#ifndef PLAYERNODE_H
#define PLAYERNODE_H
#include "MapNode.h"
#include "Image.h"
#include "Map.h"

class PlayerNode : public MapNode {
private:
	Image image;
	Map* map;
public:
	PlayerNode(Map* map);
	~PlayerNode();
    virtual void draw(Rectangle* view, Graphics* g);
};

#endif
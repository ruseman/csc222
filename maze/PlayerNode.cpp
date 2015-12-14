#include "PlayerNode.h"

PlayerNode::PlayerNode(Map* map) : MapNode(0, 14 * 32, 0 * 32, 32, 32, true, map){
	this->map = map;
	image.load("graphics.face.bmp");
}

PlayerNode::~PlayerNode(){

}

void PlayerNode::draw(Rectangle* view, Graphics* g){
	image.draw(0, 0, g);
}
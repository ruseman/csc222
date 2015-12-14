#ifndef MAP_H
#define MAP_H

#include "Graphics.h"
#include "Image.h"
#include <fstream>
#include <vector>

using namespace std;

class Map
{
private:
    Image tiles;
    int tileWidth;
    int tileHeight;
    int width;
    int height;
    int* layer1, *layer2, *layer3, *solidLayer;
    vector<int> solidTiles;

    void loadDimensions(ifstream *in);
    void loadSolidTiles(ifstream *in);
    void loadLayer(int* layer, ifstream *in);
    void generateSolidLayer();
public:
    Map();
    ~Map();

    bool load(char mapName[], char imageName[]);
    void draw(int layer, int xOffset, int yOffset, Graphics* g);
    void free();

    bool isTileSolid(int tile);
    bool checkSolid(int x, int y);

    int getWidth(){return width;};
    int getHeight(){return height;};
    int getTileWidth(){return tileWidth;};
    int getTileHeight(){return tileHeight;};
    int getTotalWidth(){return width*tileWidth;};
    int getTotalHeight(){return height*tileHeight;};
};

#endif

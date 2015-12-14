#ifndef RASTERFONT_H
#define RASTERFONT_H

#include "Image.h"

class RasterFont
{
private:
    static const int NUM_COLUMNS = 16;
    static const int START_CHAR = 32;

    Image image;
    int charSize;
public:
    bool load(char fileName[]);
    void draw(char text[], int x, int y, Graphics* g);
    void free();
};

#endif

#include "Rectangle.h"

Rectangle::Rectangle()
{
    set(0,0,1,1);
}

Rectangle::Rectangle(int ax, int ay, int aWidth, int aHeight)
{
    set(ax,ay,aWidth,aHeight);
}

Rectangle::~Rectangle()
{

}

void Rectangle::set(int ax, int ay, int aWidth, int aHeight)
{
    x = ax;
    y = ay;
    width = aWidth;
    height = aHeight;
}

void Rectangle::setPos(int ax, int ay)
{
    x = ax;
    y = ay;
}

void Rectangle::setSize(int aWidth, int aHeight)
{
    width = aWidth;
    height = aHeight;
}

void Rectangle::setX(int ax)
{
    x = ax;
}

void Rectangle::setY(int ay)
{
    y = ay;
}

void Rectangle::setWidth(int aWidth)
{
    width = aWidth;
}

void Rectangle::setHeight(int aHeight)
{
    height = aHeight;
}

void Rectangle::setX2(int x2)
{
    x = x2-width+1;
}

void Rectangle::setY2(int y2)
{
    y = y2-height+1;
}

void Rectangle::setCenterX(int ax)
{
    x = ax - width/2;
}

void Rectangle::setCenterY(int ay)
{
    y = ay - height/2;
}

int Rectangle::getX()
{
    return x;
}

int Rectangle::getY()
{
    return y;
}

int Rectangle::getWidth()
{
    return width;
}

int Rectangle::getHeight()
{
    return height;
}

int Rectangle::getX2()
{
    return x+width-1;
}

int Rectangle::getY2()
{
    return y+height-1;
}

int Rectangle::getCenterX()
{
    return x+width/2;
}

int Rectangle::getCenterY()
{
    return y+height/2;
}

bool Rectangle::overlaps(Rectangle rect)
{
    if(getX() > rect.getX2())
        return false;

    if(getY() > rect.getY2())
        return false;

    if(rect.getX() > getX2())
        return false;

    if(rect.getY() > getY2())
        return false;

    return true;
}

bool Rectangle::contains(int x, int y)
{
    if(x >= getX() && y >= getY() && x <= getX2() && y <= getY2())
        return true;

    return false;
}


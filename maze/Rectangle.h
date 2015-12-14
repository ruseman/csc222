#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle
{
private:
    int x, y, width, height;
public:
    Rectangle();
    Rectangle(int ax, int ay, int aWidth, int aHeight);
    ~Rectangle();

    void set(int ax, int ay, int aWidth, int aHeight);
    void setPos(int ax, int ay);
    void setSize(int aWidth, int aHeight);
    void setX(int ax);
    void setY(int ay);
    void setWidth(int aWidth);
    void setHeight(int aHeight);
    void setX2(int x2);
    void setY2(int y2);
    void setCenterX(int ax);
    void setCenterY(int ay);

    int getX();
    int getY();
    int getWidth();
    int getHeight();
    int getX2();
    int getY2();
    int getCenterX();
    int getCenterY();

    bool overlaps(Rectangle rect);
    bool contains(int x, int y);
};

#endif

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#define CHANNELS 3
#define PRINTPXL(x,y,c) {\
  _pixels[CHANNELS*(x+y*_w)] = c.r;\
  _pixels[CHANNELS*(x+y*_w)+1] = c.g;\
  _pixels[CHANNELS*(x+y*_w)+2] = c.b;\
}

struct Point
{
  int x, y;
  Point(int _x, int _y) : x(_x), y(_y) {}
};

struct Line
{
  int x1,y1,x2,y2;
  int y_min()
  {
    return y1 > y2 ? y1 : y2;
  }
};
struct Color
{
  unsigned char r, g, b;
  Color(unsigned char red, unsigned char green, unsigned char blue) {
    r = red; g = green; b = blue;
  }
};

#endif// __UTILS_HPP__


#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <string>
#include <fstream>
#include <iostream>

#define CHANNELS 3
#define PRINTPXL(x,y,c) {\
  _pixels[CHANNELS*(x+y*_w)] = c[0];\
  _pixels[CHANNELS*(x+y*_w)+1] = c[1];\
  _pixels[CHANNELS*(x+y*_w)+2] = c[2];\
}

class Canvas
{
 private:
   int _w, _h;
   unsigned char * _pixels;
   void scanline();
   void bresenham(int x1, int y1, int x2, int y2);
   void midptellipse(int rx, int ry, int xc, int yc);
 public:
   Canvas(int width, int height) : _w{width}, _h{height},
     _pixels{new unsigned char[width*height*CHANNELS]} {/*Empty*/}
   void draw();
   void imwrite(std::string filename);
   void rect(int width, int height, int cx=0, int cy=0);
   void line(int x1, int y1, int x2, int y2);
   void poly(int * coords);
};

#endif

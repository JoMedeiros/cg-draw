#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "utils.hpp"

class Canvas
{
 private:
   int _w, _h;
   unsigned char * _pixels;
   void scanline();
   void bresenhamline(int x1, int y1, int x2, int y2);
   void midptellipse(int rx, int ry, int xc, int yc);
   void scanline(int **lines);
 public:
   Canvas(int width, int height) : _w{width}, _h{height},
     _pixels{new unsigned char[width*height*CHANNELS]} {
       this->draw();
   }
   void draw();
   void imwrite(std::string filename);
   void rect(int width, int height, int cx=0, int cy=0);
   void line(int x1, int y1, int x2, int y2);
   void poly(int * coords);
};

#endif

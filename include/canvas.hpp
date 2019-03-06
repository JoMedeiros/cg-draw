#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
#include "utils.hpp"

#define CHANNELS 3

class Canvas
{
 private:
   int _w, _h;
   unsigned char * _pixels;
   void scanline();
   void bresenhamline(int x1, int y1, int x2, int y2, Color c);
   void bresenhamcircle(Point c, int r, Color color);
   void mirrorCircle(int xc, int yc, int x, int y, Color color);
   void midptellipse(int rx, int ry, int xc, int yc);
   void scanline(int **lines);
   void printpxl(int x, int y, Color color=Color(0,0,0));
 public:
   Canvas(int width, int height) : _w{width}, _h{height},
     _pixels{new unsigned char[width*height*CHANNELS]} {
       this->draw();
   }
   void draw();
   void imwrite(std::string filename);
   void line(int x1, int y1, int x2, int y2, Color c=Color(0,0,0));
   void line(Point pt1, Point pt2, Color c=Color(0,0,0));
   void circle(Point c, int r, Color color=Color(0,0,0));
   void rect(int _w, int _h, Point start, Color color=Color(0,0,0));
   void rect(Point topleft, Point bottomright, Color color=Color(0,0,0));
   void poly(std::vector<Point> points, Color color=Color(0,0,0));
};

#endif

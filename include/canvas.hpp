#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <string>     //std::string
#include <fstream>    //std::fstream
#include <iostream>   //std::cout
#include <vector>     //std::vector
#include <array>     //std::array
#include <list>       //std::list
#include <algorithm>  //std::sort
#include <cmath> // std::max std::min std::abs
#include "utils.hpp"

using namespace std;

#define CHANNELS 3
#define START 0
#define END 0
#define PI 3.14159265

typedef array<int, 2> StartEnd;

enum ALG {
  SCANLINE = 1,
  DDA = 2,
  BRESENHAM = 4,
  MIDPOINT = 6
};

class Canvas
{
 private:
   int _w, _h;
   unsigned char * _pixels;
   void DDA_line( int x1, int y1, int x2, int y2, Color stroke );
   void bresenhamline( int x1, int y1, int x2, int y2, Color stroke );
   void midpointline( int x1, int y1, int x2, int y2, Color stroke );
   void bresenhamcircle( Point c, int r, Color stroke );
   void mirrorCircle( int xc, int yc, int x, int y, Color stroke );
   void midPointCircle( Point c, int r, Color stroke );
   void midptellipse( int rx, int ry, int xc, int yc );
   bool printpxl( int x, int y, Color color=Color(0,0,0) );
   bool printpxl( int x, int y, Color color, float alpha ) ;
   void scanline( vector<Line> &lines, Color fill, float alpha=1 );
 public:
   Canvas(int width, int height, Color color=Color(255, 255, 255)) : 
     _w{width}, _h{height}, _pixels{new unsigned char[width*height*CHANNELS]} {
       this->draw_bg(color);
   }
   ~Canvas(){
    delete[] _pixels;
   }
  /**
   * Draws the background
   */
   void draw_bg(Color color=Color(255, 255, 255));
  /**
   * Saves the canvas as a ppm image
   */
   void imwrite(std::string filename);
  /**
   * Draws line in the canvas _pixels
   */
   void line(int x1, int y1, int x2, int y2, Color stroke, int algorithm=6);
   void circle(Point c, int r, Color stroke);
   void circle(Point c, int r, Color stroke, Color fill);
   void polyline(std::vector<Point> points, Color color=Color(0,0,0));
   void polygon(std::vector<Point> points, Color stroke);
   void polygon(std::vector<Point> points, Color stroke, Color fill, float alpha=1);
   void arc( Point center, Point start, float angle, Color stroke );
};

#endif // __CANVAS_HPP__

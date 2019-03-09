#ifndef __CANVAS_HPP__
#define __CANVAS_HPP__

#include <string>     //std::string
#include <fstream>
#include <iostream>   //std::cout
#include <cstdlib>
#include <utility>
#include <vector>     //std::vector
#include <list>       //std::list
#include <deque>      //std::deque
#include <map>
#include <utility>
#include <algorithm>  //std::sort
#include "utils.hpp"

using namespace std;

#define CHANNELS 3

class Canvas
{
 private:
   int _w, _h;
   unsigned char * _pixels;
   vector<unsigned char *> scanlines; //< Interscetion points
   void bresenhamline(int x1, int y1, int x2, int y2, Color c, bool scanline=false);
   void bresenhamcircle(Point c, int r, Color color);
   void mirrorCircle(int xc, int yc, int x, int y, Color color);
   void midptellipse(int rx, int ry, int xc, int yc);
   void print_scanline(unsigned char *start, unsigned char *end, Color color);
   bool printpxl(int x, int y, Color color=Color(0,0,0));
   unsigned char * get_pos(int x, int y);
 public:
   Canvas(int width, int height, Color color=Color(255, 255, 255)) : 
     _w{width}, _h{height}, _pixels{new unsigned char[width*height*CHANNELS]} {
       this->draw_bg(color);
   }
   ~Canvas(){
    delete _pixels;
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
   void line(int x1, int y1, int x2, int y2, Color c=Color(0,0,0), bool scanline=false);
   void line(Point pt1, Point pt2, Color c=Color(0,0,0), bool scanline=false);
   void circle(Point c, int r, Color color=Color(0,0,0));
   void rect(int _w, int _h, Point start, Color color=Color(0,0,0));
   void rect(Point topleft, Point bottomright, Color color=Color(0,0,0));
   void polyline(std::vector<Point> points, Color color=Color(0,0,0));
   void polygon(std::vector<Point> points, Color color=Color(0,0,0));
   //void scanline(std::vector<Point> points, Color color=Color(0,0,0));
};

#endif // __CANVAS_HPP__

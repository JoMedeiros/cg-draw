#include "canvas.hpp"

// @TODO General: avoid printing pixel outside the canvas

void Canvas::draw_bg(Color color) {
  // @TODO Call draw function in a vector of drawable entities?
  // Initializing background
  for ( int i = 0; i < _w; ++i )
    for ( int j = 0; j < _h; ++j )
      printpxl(i, j , color);
}

void Canvas::imwrite(std::string filename) {
  std::fstream fs (filename, std::fstream::out);
  if (!fs) 
    std::cout << "Error loading file\n";
  fs << "P3\n" << _w << " " << _h << "\n" << "255\n" ;
  for ( int i = 0; i < _w*_h*3; ++i )
    fs << (int) _pixels[i] << "\n";
  fs.close();
}
/**
 * Draws line in the canvas _pixels
 */
void Canvas::line(Point pt1, Point pt2, Color color, bool scanline) {
  bresenhamline(pt1.x, pt1.y, pt2.x, pt2.y, color, scanline);
}

void Canvas::line(int x1, int y1, int x2, int y2, Color c, bool scanline) {
  bresenhamline(x1, y1, x2, y2, c, scanline);
}
/**
 * Draws circle in the canvas _pixels
 */
void Canvas::circle(Point c, int r, Color color) {
  bresenhamcircle(c,r,color);
}
/**
 * Bresenham Algorithm to draw circles
 */
void Canvas::bresenhamcircle(Point c, int r, Color color) {
  int x = 0, y = r;
  int d = 3 - 2 * r;
  mirrorCircle(c.x, c.y, x, y, color);
  while (y >= x) {
    ++x;
    if (d > 0){
      --y;
      d = d + 4 * (x - y) + 10;
    }
    else
      d = d + 4 * x + 6;
    mirrorCircle(c.x, c.y, x, y, color);
  }
}
/**
 * Draws tge other 8 mirrored points of the cricle
 */
void Canvas::mirrorCircle(int xc, int yc, int x, int y, Color color) {
  printpxl(xc+x, yc+y, color);
  printpxl(xc-x, yc+y, color);
  printpxl(xc+x, yc-y, color);
  printpxl(xc-x, yc-y, color);
  printpxl(xc+y, yc+x, color);
  printpxl(xc-y, yc+x, color);
  printpxl(xc+y, yc-x, color);
  printpxl(xc-y, yc-x, color);
}
/**
 * Bresenham Algorithm to draw lines
 */
void Canvas::bresenhamline(int x1, int y1, int x2, int y2, Color c, bool scanline) {
  bool increase_x = abs(x1 - x2) >= abs(y1 - y2);
  int m_new = 2 * abs(y2 - y1);
  int slope_error_new = m_new - abs(x2 - x1);
  if (increase_x){
    if (x1 > x2) {std::swap(x1,x2);std::swap(y1,y2);}
    for (int x = x1, y = y1; x <= x2; ++x) {
      bool res = printpxl(x,y,c);
      slope_error_new += m_new;
      if (slope_error_new >= 0) {
        // @TODO: push pos(x, y) after y change
        if (scanline and res) {
          scanlines.push_back( get_pos(x, y) );
        }
        if (y1 < y2) ++y;
        else --y;
        slope_error_new -= 2 * (x2 - x1);
      }
    }
  }
  else {
    m_new = 2 * abs(x2 - x1);
    slope_error_new = m_new - (y2 - y1);
    if (y1 > y2) {std::swap(x1,x2);std::swap(y1,y2);}
    for (int y = y1, x = x1; y <= y2; ++y) {
      bool res = printpxl(x,y,c);
      // @TODO: push pos(x, y) after y change
      if (scanline and res) {
        scanlines.push_back( get_pos(x, y) );
      }
      slope_error_new += m_new;
      if (slope_error_new >= 0) {
        if (x1 < x2) ++x;
        else --x;
        slope_error_new -= 2 * (y2 - y1);
      }
    }
  }
}
/**
 * A simple algorithm to draw rectangle
 */
void Canvas::rect(int _w, int _h, Point start, Color color) {
  int x = start.x, y = start.y;
  for ( ; x <= (start.x+_w); ++x)
    printpxl(x, y, color);
  for ( ; y <= (start.y+_h); ++y)
    printpxl(x, y, color);
  for ( ; x >= (start.x); --x)
    printpxl(x, y, color);
  for ( ; y >= (start.y); --y)
    printpxl(x, y, color);
}

void Canvas::rect(Point topleft, Point bottomright, Color color) {
  int _w = bottomright.x - topleft.x;
  int _h = bottomright.y - topleft.y;
  rect(_w, _h, topleft, color);
}
/**
 * Fill algorithm scanline
 * \param lines a list o coordenates
 */
void Canvas::polyline(std::vector<Point> points, Color color) {
  for (int i = 1; i < points.size(); ++i) 
    line(points[i-1], points[i], color);
}

//@TODO modify the command line to store the points if scanline fill is set.
void Canvas::polygon(std::vector<Point> points, Color color) {
  size_t n = points.size();
  for (int i = 1; i < n; ++i) 
    line(points[i-1], points[i], color, true);
  line(points[0], points[n-1], color, true);
  //Scanline fill
  sort(scanlines.begin(), scanlines.end());
  for ( int i=0; i < scanlines.size()+1; i+=2) {
    auto start = scanlines[i];
    auto end = scanlines[i+1];
    print_scanline(start, end, Color(255,0,0));
  }
  scanlines.clear();
}
/**
 * Fill algorithm scanline
 * \param lines a list o points of the polygon
 */
void Canvas::print_scanline(unsigned char* start, unsigned char* end, Color color) {
  for(auto it = start; it < end; it+=3) {
    *it = color.r;
    *(it+1) = color.g;
    *(it+2) = color.b;
  }
}

bool Canvas::printpxl(int x, int y, Color color) {
  if (x > _w or y > _h or x < 0 or y < 0) {
    //std::cerr << "point out of boundaries\n";
    return false;
  }
  _pixels[(y*_w + x)*CHANNELS] = color.r;
  _pixels[(y*_w + x)*CHANNELS + 1] = color.g;
  _pixels[(y*_w + x)*CHANNELS + 2] = color.b;
  return true;
}

unsigned char * Canvas::get_pos(int x, int y) {
  return &_pixels[(y*_w + x)*CHANNELS];
}


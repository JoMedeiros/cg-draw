#include "canvas.hpp"
// @TODO General: avoid printing pixel outside the canvas
/**
 * Draws the background
 */
void Canvas::draw() {
  // @TODO Call draw function in a vector of drawable entities?
  Color BG = Color(255,255,255);
  // Initializing background
  for ( int i = 0; i < _w; ++i )
    for ( int j = 0; j < _h; ++j )
      printpxl(i, j , BG);
}
/**
 * Draws line in the canvas _pixels
 */
void Canvas::line(int x1, int y1, int x2, int y2, Color c) {
  bresenhamline(x1, y1, x2, y2, c);
}
/**
 * Draws line in the canvas _pixels
 */
void Canvas::line(Point pt1, Point pt2, Color color)
{
  bresenhamline(pt1.x, pt1.y, pt2.x, pt2.y, color);
}
/**
 * Draws circle in the canvas _pixels
 */
void Canvas::circle(Point c, int r, Color color)
{
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
void Canvas::bresenhamline(int x1, int y1, int x2, int y2, Color c) {
  //@TODO change function to take Color as a parameter
  bool increase_x = abs(x1 - x2) >= abs(y1 - y2);
  int m_new = 2 * abs(y2 - y1);
  int slope_error_new = m_new - abs(x2 - x1);
  if (increase_x){
    if (x1 > x2) {std::swap(x1,x2);std::swap(y1,y2);}
    for (int x = x1, y = y1; x <= x2; ++x) {
      printpxl(x,y,c);
      slope_error_new += m_new;
      if (slope_error_new >= 0) {
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
      printpxl(x,y,c);
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

void Canvas::polyline(std::vector<Point> points, Color color) {
  for (int i = 1; i < points.size(); ++i) 
    line(points[i-1], points[i], color);
}
void Canvas::polygon(std::vector<Point> points, Color color) {
  size_t n = points.size();
  for (int i = 1; i < n; ++i) 
    line(points[i-1], points[i], color);
  line(points[0], points[n-1], color);
}
/**
 * Fill algorithm scanline
 * \param lines a list o points of the polygon
 */
void Canvas::scanline(std::vector<Point> points, Color color)
{
  //@TODO ordenar as linhas por y_min
  // - ideia: criar uma classe util de fraction para facilitar os
  // calculos com o coeficiente angular
  size_t n = points.size();
  std::deque<Line> lines(n);
  for (int i = 1; i < n; ++i)
    lines[i] = Line(points[i-1], points[i]);
  lines[0] = Line(points[0], points[n-1]);
  // Sorting the lines by y_min
  std::sort(lines.begin(), lines.end(), Line::comp_ymin);
  int yi = lines[0].y_min();
  int yf = lines[0].y_max();
  for (auto it = lines.begin(); it != lines.end(); ++it){
    yf = (it->y_max() > yf) ? it->y_max() : yf;
  }
  // @TODO modify it to not use m as float
  std::deque<Line> work;
  int dy = 0;
  // For each scanline do:
  for (;yi < yf; ++yi) {
    std::deque<Point> sl_pts;
    while ( lines.front().y_min() < yi ) {
      work.push_front(lines.front());
      lines.pop_front();
    }
    for (auto it=work.begin(); it != work.end(); ++it){
      // If scanline is higher than the line, remove it
      if (it->y_max() < yi) {
        work.erase(it);
      }
      // Else, calculate the intersections
      else {
        std::cout << "line inclination: " << it->m_i << "\n";
      }
    }
    ++dy;
  }
}
/**
 * Saves the canvas as a ppm image
 */
void Canvas::imwrite(std::string filename) {
  //this->draw();
  std::fstream fs (filename, std::fstream::out);
  if (!fs) 
    std::cout << "Error loading file\n";
  fs << "P3\n" << _w << " " << _h << "\n" << "255\n" ;
  for ( int i = 0; i < _w*_h*3; ++i )
    fs << (int) _pixels[i] << "\n";
  fs.close();
}
void Canvas::printpxl(int x, int y, Color color)
{
  if (x > _w or y > _h or x < 0 or y < 0) {
    //std::cerr << "point out of boundaries\n";
    return;
  }
  _pixels[(y*_w + x)*CHANNELS] = color.r;
  _pixels[(y*_w + x)*CHANNELS + 1] = color.g;
  _pixels[(y*_w + x)*CHANNELS + 2] = color.b;
}

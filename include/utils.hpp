#ifndef __UTILS_HPP__
#define __UTILS_HPP__

struct Point
{
  int x, y;
  Point(int _x, int _y) : x(_x), y(_y) {}
};

// @TODO Try to resolve scanline without float m_i
struct Line
{
  Point start, end;
  float m_i;
  int y_min() const {
    return start.y < end.y ? start.y : end.y;
  }
  int y_max() {
    return start.y > end.y ? start.y : end.y;
  }
  Point y_min_pt() {
    return start.y < end.y ? start : end;
  }
  Line(int x1, int y1, int x2, int y2): start(x1, y1), end(x2, y2) {
    if ((y1-y2) == 0) m_i = 0;
    else m_i = (float)(x1 - x2)/(y1 -y2);
  }
  Line(Point s=Point(0,0), Point e=Point(0,0)): start(s), end(e) {
    if ((s.y-e.y) == 0) m_i = 0;
    else m_i = (float)(s.x - e.x)/(s.y - e.y);
  }
  bool operator<(/*const Line& l,*/ Line& r) {
    return this->y_min() < r.y_min();
  }
  static bool comp_ymin( Line& l,  Line& r){
    return l.y_min() < r.y_min();
  }
};

struct Color
{
  unsigned char r, g, b;
  Color(unsigned char red=0, unsigned char green=0, unsigned char blue=0) {
    r = red; g = green; b = blue;
  }
  Color(const Color &other) {
    r = other.r; g = other.g; b = other.b;
  }
};

#endif// __UTILS_HPP__


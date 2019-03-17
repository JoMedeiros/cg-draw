#ifndef __UTILS_HPP__
#define __UTILS_HPP__

struct Point
{
  int x, y;
  Point(int _x=0, int _y=0) : x(_x), y(_y) {}
  Point( const Point& pt ) : x(pt.x), y(pt.y) {}
};

// @TODO Try to resolve scanline without float m_i
struct Line
{
  Point start, end;
  double m_i;
  int y_min() const {
    return start.y;
  }
  int y_max() {
    return end.y;
  }
  Point y_min_pt() {
    return start.y < end.y ? start : end;
  }
  Line(int x1, int y1, int x2, int y2) {
    if ( y1 < y2 ) {
      start = Point( x1, y1 );
      end = Point( x2, y2 );
      if ((y1-y2) == 0) m_i = 0;
      else m_i = (double)(x2 - x1)/(double)(y2 -y1);
    }
    else {
      start = Point( x2, y2 );
      end = Point( x1, y1 );
      if ((y1-y2) == 0) m_i = 0;
      else m_i = (float)(x1 - x2)/(y1 -y2);
    }
  }
  Line(Point s=Point(0,0), Point e=Point(0,0)) {
    if ( s.y < e.y ) {
      start = Point( s.x, s.y );
      end = Point( e.x, e.y );
      if ((s.y-e.y) == 0) m_i = 0;
      else m_i = (double)(e.x - s.x)/(double)(e.y -s.y);
    }
    else {
      start = Point( e.x, e.y );
      end = Point( s.x, s.y );
      if ((s.y-e.y) == 0) m_i = 0;
      else m_i = (float)(s.x - e.x)/(s.y -e.y);
    }
  }
  bool operator<(/*const Line& l,*/ Line& r) {
    return this->y_min() < r.y_min();
  }
  bool operator>(/*const Line& l,*/ Line& r) {
    return this->y_min() > r.y_min();
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

/*Color Color::rgb2hsv(Color & c) {
  int h,s,v;
  int _max;
  int _min;
  if (c.r > c.b) {
    if (c.r > c.g) {
      _max = c.r;
      h = 60 * (0 + (c.g - c.b)/(_max - _min));
    }
    else {
      _max = c.g;
      h = 60 * (2 + (c.b - c.r)/(_max - _min));
    }
  }
  else {
    if (c.g > c.b) {
      _max = c.g;
      h = 60 * (2 + (c.b - c.r)/(_max - _min));
    }  
    else {
      _max = c.b;
      h = 60 * (4 + (c.r - c.g)/(_max - _min));
    }
  }
  s = _max > 0 ? (_max - _min)/_max : 1;//if v=0, s=0
  v = _max;
  return Color(h, s, v);
}*/


#endif// __UTILS_HPP__


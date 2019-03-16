#include "canvas.hpp"

void Canvas::draw_bg(Color color) {
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

void Canvas::line(int x1, int y1, int x2, int y2, Color stroke, int algorithm) {
  bool scanline = algorithm & ALG::SCANLINE;
  switch (algorithm ^ 1){
    case ALG::BRESENHAM:
      bresenhamline(x1, y1, x2, y2, stroke, scanline);
      break;
    case ALG::DDA:
      DDA_line(x1, y1, x2, y2, stroke, scanline);
      break;
    default: 
      midpointline( x1, y1, x2, y2, stroke, scanline);
      break;
  }
}

void Canvas::DDA_line( int x1, int y1, int x2, int y2, Color c, bool scan) {
  int dx = (x2 - x1);
  int dy = (y2 - y1);
  int steps = max(abs(dx), abs(dy));
  float x_increment = dx / (float) steps;
  float y_increment = dy / (float) steps;
  float x = x1, y = y1;
  for ( int i=0; i <= steps; ++i) {
    //int prev_y = round(y);
    printpxl(round(x), round(y), c);
    x = x + x_increment;
    y = y + y_increment;
    //if (scanline and round(y) != prev_y) scanlines.push_back( get_pos(x, y) );
  }
}
void Canvas::bresenhamline( int x1, int y1, int x2, int y2, Color c, bool scan) {
  int dx = abs(x2 - x1), dy = abs(y2 - y1); // dx = 1, dy = 0
  int ix = x2 >= x1 ? 1:-1, iy = y2 >= y1 ? 1:-1;// ix = 1, iy = 1
  int x = x1, y = y1;// x = 1, y = 1
  if (abs(dx) >= abs(dy)) {
    int D = dy - dx;// D = -1
    for (;x != (x2+ix); x+=ix) {
      printpxl(x, y, c);
      if (D >= 0){
        y += iy;
        D -= dx;
      }
      D += dy;
    }
  }
  else {
    int D = dx - dy;
    for (;y != (y2+iy); y+=iy) {
      printpxl(x, y, c);
      if (D >= 0){
        x += ix;
        D -= dy;
      }
      D += dx;
    }
  }
}
void Canvas::midpointline( int x1, int y1, int x2, int y2, Color c , bool scan){
  int dx = abs(x2 - x1), dy = abs(y2 - y1);
  int d = dx > dy ? dy - (dx/2) : dx - (dy/2);
  int x = x1, y = y1;
  int xi = (x2 > x1 ? 1 : -1), yi = (y2 > y1 ? 1 : -1);
  printpxl(x, y, c);
  if (abs(dx) >= abs(dy)) {
    while ( x != x2) {
      x += xi;
      if (d < 0) d += dy;
      else {
        d += (dy - dx);
        y += yi;
        if (y - y_min > 0 and y - y_min < scanline_points.size() and
            scan) scanline_points[y - y_min].push_back(x);
      }
      printpxl(x, y, c);
    }
  }
  else {
    while ( y != y2) {
      y += yi;
      if (d < 0) d += dx;
      else {
        d += (dx - dy);
        x += xi;
      }
      if (y - y_min > 0 and y - y_min < scanline_points.size() and 
          scan) scanline_points[y - y_min].push_back(x);
      printpxl(x, y, c);
    }
  }
}
/**
 * Draws circle in the canvas _pixels
 */
void Canvas::circle(Point c, int r, Color stroke, Color fill) {
  bresenhamcircle(c,r,stroke);
  // @TODO fill algorithm
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
 * Fill algorithm scanline
 * \param lines a list o coordenates
 */
void Canvas::polyline(std::vector<Point> points, Color color) {
  for (int i = 1; i < points.size(); ++i) {
    line( points[i-1].x, points[i-1].y, points[i].x, points[i].y, color);
  }
}

//@TODO modify the command line to store the points if scanline fill is set.
void Canvas::polygon(std::vector<Point> points, Color stroke, Color fill) {
  polygon(points, stroke);
  //Scanline fill
  for (int i=0; i < scanline_points.size(); ++i) {
    cout << "Line " << y_min + i << "\n";
    print_scanline(scanline_points[i]);
  }
  scanline_points.clear();
}
void Canvas::polygon(std::vector<Point> points, Color stroke) {
  // @TODO Fix the problem of the scanline flag
  size_t n = points.size();
  // Find max and min y values for scanline
  y_min = points[0].y;
  y_max = points[0].y;
  for (int i = 1; i < n; ++i) {
    if (points[i].y > y_max) y_max = points[i].y;
    if (points[i].y < y_min) y_min = points[i].y;
  }
  scanline_points.resize(y_max - y_min + 1);
  // Draw lines
  for (int i = 1; i < n; ++i) {
    line(points[i-1].x, points[i-1].y, points[i].x, points[i].y,
        stroke, ALG::SCANLINE | ALG::MIDPOINT);
  }
  line( points[0].x, points[0].y, points[n-1].x, points[n-1].y,
      stroke, ALG::SCANLINE | ALG::MIDPOINT);
}
/**
 * Fill algorithm scanline
 * \param lines a list o points of the polygon
 */
void Canvas::print_scanline(unsigned char* start, unsigned char* end, Color color) {
  // @TODO paint only a pair number per line (y)
  for(auto it = start+3; it < end; it+=3) {
    *it = color.r;
    *(it+1) = color.g;
    *(it+2) = color.b;
  }
}
void Canvas::print_scanline(vector<int> xs) {
//@TODO sort the xs and start to print
  sort(xs.begin(), xs.end());
  cout << "All xs: ";
  for (int i=0; i < xs.size(); i+=2){
    //cout << "Intersection pair: " << "( " << xs[i-1] << ", " << xs[i] << " )\n";
    cout << xs[i] << ", " << xs[i+1];
    //++i;
    //for 
    //print_scanline( get_pos(xs[i], y_min+i ), get_pos(xs[i+1], y_min+i), Color(10,80,255) );
    
  }
  cout << "\n";
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
  return &_pixels[((y)*_w + x)*CHANNELS];
}


#include "canvas.hpp"
//@TODO create a list of horizontal lines to remove every x in scanlines between
//that line

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
      bresenhamline( x1, y1, x2, y2, stroke );
      break;
    case ALG::DDA:
      DDA_line( x1, y1, x2, y2, stroke );
      break;
    default: 
      midpointline( x1, y1, x2, y2, stroke );
      break;
  }
}

void Canvas::DDA_line( int x1, int y1, int x2, int y2, Color stroke ) {
  int dx = (x2 - x1);
  int dy = (y2 - y1);
  int steps = max(abs(dx), abs(dy));
  float x_increment = dx / (float) steps;
  float y_increment = dy / (float) steps;
  float x = x1, y = y1;
  for ( int i=0; i <= steps; ++i) {
    // Antialiasing
    for (int xi=floor(x); xi <= ceil(x);++xi) {
      for (int yi=floor(y); yi <= ceil(y); ++yi) {
        float percent_x = 1 - abs ( x - xi );
        float percent_y = 1 - abs ( y - yi );
        float alpha = percent_x * percent_y;
        if (alpha > 0.20)
          printpxl( xi, yi, stroke, alpha );
      }
    }
    x = x + x_increment;
    y = y + y_increment;
  }
}
void Canvas::bresenhamline( int x1, int y1, int x2, int y2, Color stroke ) {
  int dx = abs(x2 - x1), dy = abs(y2 - y1); // dx = 1, dy = 0
  int ix = x2 >= x1 ? 1:-1, iy = y2 >= y1 ? 1:-1;// ix = 1, iy = 1
  int x = x1, y = y1;// x = 1, y = 1
  if (abs(dx) >= abs(dy)) {
    int D = dy - dx;// D = -1
    for (;x != (x2+ix); x+=ix) {
      printpxl( x, y, stroke );
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
      printpxl( x, y, stroke );
      if (D >= 0){
        x += ix;
        D -= dy;
      }
      D += dx;
    }
  }
}
void Canvas::midpointline( int x1, int y1, int x2, int y2, Color stroke ){
  int dx = abs(x2 - x1), dy = abs(y2 - y1);
  int d = dx > dy ? dy - (dx/2) : dx - (dy/2);
  int x = x1, y = y1;
  int xi = (x2 > x1 ? 1 : -1), yi = (y2 > y1 ? 1 : -1);
  printpxl( x, y, stroke );

  if (abs(dx) >= abs(dy)) {
    while ( x != x2) {
      x += xi;
      if (d < 0) d += dy;
      else {
        d += (dy - dx);
        y += yi;
      }
      printpxl( x, y, stroke );
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
      printpxl( x, y, stroke );
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

void Canvas::polygon(std::vector<Point> points, Color stroke, Color fill, float alpha) {
  //Scanline fill
  vector<Line> lines;
  for (int i=1; i < points.size(); ++i) {
    lines.push_back( Line(points[i-1], points[i]) );
  }
  lines.push_back( Line( points[0], points.back() ) );
  scanline( lines, fill, alpha );
  //Lines
  polygon(points, stroke);
}
void Canvas::scanline( vector<Line> &lines, Color fill, float alpha ) {
  sort( lines.begin(), lines.end() );// vector is faster in sort algorithm
  list<Line> lines_lst(lines.begin(), lines.end());//list is faster to remove randomly
  int y_scan = lines.front().y_min();
  vector<int> xs;
  while (not lines_lst.empty()){
    bool stop = false;
    for ( auto it = lines_lst.begin(); it != lines_lst.end() 
        and it->y_min() <= y_scan; ++it ) 
    {
      if ( (it->y_min() - it->y_max()) == 0 or it->y_max() <= y_scan ){
        it = lines_lst.erase(it);
      }
      int x = it->start.x + ((y_scan - it->y_min()) * it->m_i);
      xs.push_back(x);
    }
    sort( xs.begin(), xs.end() );
    if (xs.size()%2 == 0 and xs.size() != 0 ){
      for (auto it=xs.begin(); it != xs.end(); ++it){
        auto it1 = it;
        auto it2 = ++it;
        for (int x=(*it1)+1; x <= *it2; ++x)
          printpxl( x, y_scan, fill, alpha );
      }
      ++y_scan;
    }
    xs.clear();
  }
}
void Canvas::polygon(std::vector<Point> points, Color stroke) {
  size_t n = points.size();
  // Draw lines
  for (int i = 1; i < n; ++i) {
    line(points[i-1].x, points[i-1].y, points[i].x, points[i].y,
        stroke, ALG::SCANLINE | ALG::DDA);
  }
  line( points[0].x, points[0].y, points[n-1].x, points[n-1].y,
      stroke, ALG::SCANLINE | ALG::DDA);
}
/**
 * Draws a arc in counterclockwise degrees from a start.
 * @param center The center of the arc
 * @param start Start point
 * @param angle The angle in degrees
 */
void Canvas::arc( Point center, Point start, int angle, Color stroke ) {
  // Calculate radius
  int dx = start.x - center.x;
  int dy = start.y - center.y;
  int r = sqrt( dx*dx + dy*dy );
  cout << "dx: " << dx << "\n";
  cout << "dy: " << dy << "\n";
  cout << "Raio: " << r << "\n";
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

bool Canvas::printpxl( int x, int y, Color color, float alpha ) {
  if (x > _w or y > _h or x < 0 or y < 0) {
    //std::cerr << "point out of boundaries\n";
    return false;
  }
  // Calculating new colors
  unsigned char bg_r = _pixels[(y*_w + x)*CHANNELS];
  unsigned char bg_g = _pixels[(y*_w + x)*CHANNELS + 1];
  unsigned char bg_b = _pixels[(y*_w + x)*CHANNELS + 2];
  unsigned char out_r = alpha * color.r + (1 - alpha) * bg_r;
  unsigned char out_g = alpha * color.g + (1 - alpha) * bg_g;
  unsigned char out_b = alpha * color.b + (1 - alpha) * bg_b;

  _pixels[(y*_w + x)*CHANNELS] = out_r;
  _pixels[(y*_w + x)*CHANNELS + 1] = out_g;
  _pixels[(y*_w + x)*CHANNELS + 2] = out_b;

  return true;
}


/**
 * @file    canvas.cpp
 * @brief   Implementation of Canvas class. 
 *
 * @author  Josivan Medeiros da Silva Gois
 * @date    18/03/2019
 */
#include "canvas.hpp"

/**
 * @brief     Initialize the background with a especified color.
 * @param color The color of the background.
 */
void Canvas::draw_bg(Color color) {
  // Initializing background
  for ( int i = 0; i < _w; ++i )
    for ( int j = 0; j < _h; ++j )
      printpxl(i, j , color);
}
/**
 * @brief     Write a ppm image in a especified file name.
 * @param Name for the output file.
 */
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
 * @brief   Draws a line on the canvas.
 * @param x1  The x coordinate for the start point.
 * @param y1  The y coordinate for the start point.
 * @param x2  The x coordinate for the end point.
 * @param y2  The y coordinate for the end point.
 * @param stroke  The color of the line.
 * @param algorithm A enumeration of the algorithm used to draw the line.
 */
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
/**
 * @brief   Draws a line on the canvas using Digital differencial analyzer 
 * algorithm.
 * @param x1  The x coordinate for the start point.
 * @param y1  The y coordinate for the start point.
 * @param x2  The x coordinate for the end point.
 * @param y2  The y coordinate for the end point.
 * @param stroke  The color of the line.
 */
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
/**
 * @brief   Draws a line on the canvas using Mid-Point generation algorithm.
 * @param x1  The x coordinate for the start point.
 * @param y1  The y coordinate for the start point.
 * @param x2  The x coordinate for the end point.
 * @param y2  The y coordinate for the end point.
 * @param stroke  The color of the line.
 */
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
 * @brief   Draws a line on the canvas using Bresenham's line algorithm.
 * @param x1  The x coordinate for the start point.
 * @param y1  The y coordinate for the start point.
 * @param x2  The x coordinate for the end point.
 * @param y2  The y coordinate for the end point.
 * @param stroke  The color of the line.
 */
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
/**
 * @brief   Draws circle in the canvas _pixels
 * @param c A Point object. The center of the circle
 * @param r The radius of the circle.
 * @param stroke  The color of the line.
 */
void Canvas::circle(Point c, int r, Color stroke) {
  bresenhamcircle(c,r,stroke);
}
/**
 * @brief   Draws circle in the canvas _pixels
 * @param c A Point object. The center of the circle
 * @param r The radius of the circle.
 * @param stroke  The color of the line.
 * @param fill  The color to fill the circle.
 */
void Canvas::circle(Point c, int r, Color stroke, Color fill) {
  bresenhamcircle(c,r,stroke);
  // @TODO fill algorithm
}
/**
 * @brief   Bresenham Algorithm to draw circles
 * @param c A Point object. The center of the circle
 * @param r The radius of the circle.
 * @param stroke  The color of the line.
 */
void Canvas::bresenhamcircle( Point c, int r, Color stroke ) {
  int x = 0, y = r;
  int d = 3 - 2 * r;
  mirrorCircle(c.x, c.y, x, y, stroke);
  while (y >= x) {
    ++x;
    if (d > 0){
      --y;
      d = d + 4 * (x - y) + 10;
    }
    else
      d = d + 4 * x + 6;
    mirrorCircle(c.x, c.y, x, y, stroke);
  }
}
/**
 * @brief   Bresenham Algorithm to draw circles
 * @param c A Point object. The center of the circle
 * @param r The radius of the circle.
 * @param stroke  The color of the line.
 */
void Canvas::midPointCircle( Point c, int r, Color stroke ) {
  int cx = c.x, cy = c.y;
  int x = r, y = 0; 
  printpxl( x + cx, y + cy, stroke); 
  if (r > 0) {
    printpxl( x + cx, -y + cy, stroke);
    printpxl( y + cx, x + cy, stroke);
    printpxl( -y + cx, x + cy, stroke);
  }
  int P = 1 - r;
  while (x > y) {
    y++;
    if (P <= 0)
      P = P + 2*y + 1; 
    else {
      x--; 
      P = P + 2*y - 2*x + 1; 
    }
    if (x < y) 
      break; 
    printpxl( x + cx, y + cy, stroke );
    printpxl( -x + cx, y + cy, stroke );
    printpxl( x + cx, -y + cy, stroke );
    printpxl( -x + cx, -y + cy, stroke );
    if (x != y) { 
      printpxl( y + cx, x + cy, stroke );
      printpxl( -y + cx, x + cy, stroke );
      printpxl( y + cx, -x + cy, stroke );
      printpxl( -y + cx, -x + cy, stroke );
    }
  }
}
/**
 * Draws the other 8 mirrored points of the cricle mirroring for the others
 * sectors.
 * @param xc The center of the circle x coordinate.
 * @param yc The center of the circle x coordinate.
 * @param x  The x coordinate increment/decrement.
 * @param y  The x coordinate increment/decrement.
 * @param stroke  The color of the line.
 */
void Canvas::mirrorCircle(int xc, int yc, int x, int y, Color stroke) {
  printpxl(xc+x, yc+y, stroke);
  printpxl(xc-x, yc+y, stroke);
  printpxl(xc+x, yc-y, stroke);
  printpxl(xc-x, yc-y, stroke);
  printpxl(xc+y, yc+x, stroke);
  printpxl(xc-y, yc+x, stroke);
  printpxl(xc+y, yc-x, stroke);
  printpxl(xc-y, yc-x, stroke);
}
/**
 * @brief Function to draw polyline.
 * @param points  The points of the polyline in order.
 * @param stroke  The line color.
 */
void Canvas::polyline(std::vector<Point> points, Color stroke) {
  for (int i = 1; i < points.size(); ++i) {
    line( points[i-1].x, points[i-1].y, points[i].x, points[i].y, stroke);
  }
}
/**
 * @brief Function to draw polygons.
 * @param points  The points of the polygon in order.
 * @param stroke  The line color.
 */
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
/**
 * @brief Scanline fill algorithm for polygons.
 * @param lines A list of a polygon lines.
 * @param fill  The color to fill.
 * @param alpha Alpha component of the color (to blend with the background).
 */
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
/**
 * @brief Function to draw polygons.
 * @param points  The points of the poligon in order.
 * @param stroke  The line color.
 */
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
 * @param stroke  The line color.
 */
void Canvas::arc( Point center, Point start, float angle, Color stroke ) {
  // Calculate radius
  int dx = start.x - center.x;
  int dy = start.y - center.y;
  int r = sqrt( dx*dx + dy*dy );
  int start_angle;
  if (dx == 0){
    if (dy > 0)
      start_angle = PI/2.0;
    else
      start_angle = 3*PI/2.0;
  }
  else if ( dy == 0) {
    if (dx > 0)
      start_angle = 0;
    else
      start_angle = PI;
  }
  start_angle = ((float)dx/dy);
  float end_angle = start_angle + angle;
  
  // Midpoint Algorithm
  int cx = center.x;
  int cy = center.y;
  int x = r, y = 0; 

  float c_ang = (float) ((x + cx) - center.x)/(cy - center.y);
  if ( c_ang > start_angle and c_ang < end_angle )
    printpxl( x + cx, cy, stroke ); 
    
  if (r > 0) {
    c_ang = (float) ((x + cx) - center.x)/((-y + cy) - center.y);
    if ( c_ang >= start_angle and c_ang <= end_angle )
      printpxl( x + cx, -y + cy, stroke );
    c_ang = (float) ((y + cx) - center.x)/((x + cy) - center.y);
    if ( c_ang >= start_angle and c_ang < end_angle )
      printpxl( y + cx, x + cy, stroke );
    c_ang = (float) ((-y + cx) - center.x)/((x + cy) - center.y);
    if ( c_ang > start_angle and c_ang < end_angle )
      printpxl( -y + cx, x + cy, stroke );
  }

  int P = 1 - r; 
  while (x > y) {  
    y++; 
    if (P <= 0) 
      P = P + 2*y + 1; 
    else { 
      x--; 
      P = P + 2*y - 2*x + 1; 
    } 
    if (x < y) 
      break; 
    c_ang = (float) ((x + cx) - center.x)/((y + cy) - center.y);
    if ( c_ang > start_angle and c_ang < end_angle )
      printpxl( x + cx, y + cy, stroke );
    c_ang = (float) ((-x + cx) - center.x)/((y + cy) - center.y);
    if ( c_ang > start_angle and c_ang < end_angle )
      printpxl( -x + cx, y + cy, stroke );
    c_ang = (float) ((x + cx) - center.x)/((-y + cy) - center.y);
    if ( c_ang > start_angle and c_ang < end_angle )
      printpxl( x + cx, -y + cy, stroke );
    c_ang = (float) ((-x + cx) - center.x)/((-y + cy) - center.y);
    if ( c_ang > start_angle and c_ang < end_angle )
      printpxl( -x + cx, -y + cy, stroke );
    // If the generated point is on the line x = y then  
    // the perimeter points have already been printed 
    if (x != y) {
      c_ang = (float) ((y + cx) - center.x)/((x + cy) - center.y);
      if ( c_ang > start_angle and c_ang < end_angle )
        printpxl( y + cx, x + cy, stroke );
      c_ang = (float) ((-y + cx) - center.x)/((x + cy) - center.y);
      if ( c_ang > start_angle and c_ang < end_angle )
        printpxl( -y + cx, x + cy, stroke );
      c_ang = (float) ((y + cx) - center.x)/((-x + cy) - center.y);
      if ( c_ang > start_angle and c_ang < end_angle )
        printpxl( y + cx, -x + cy, stroke );
      c_ang = (float) ((-y + cx) - center.x)/((-x + cy) - center.y);
      if ( c_ang > start_angle and c_ang < end_angle )
        printpxl( -y + cx, -x + cy, stroke );
    }
  }
}
/**
 * @brief Changes the value in the matrix of pixels corresponding to coordinate.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param color The color to be assigned to pixel.
 */
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
/**
 * @brief Changes the value in the matrix of pixels corresponding to coordinate.
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @param color The color to be assigned to pixel.
 * @param alpha The value of alpha (transparancy) to blend with the background.
 */
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


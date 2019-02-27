#include "canvas.hpp"

void Canvas::draw() {
  // @TODO Call draw function in a vector of drawable entities?
  Color BG = Color(255,255,255);
  // Initializing background
  for ( int i = 0; i < _w; ++i )
    for ( int j = 0; j < _h; ++j )
      PRINTPXL(i, j , BG);
  // Draw functions
  line(10,10, 100, 250);
}
/**
 * Draws line in the canvas _pixels
 */
void Canvas::line(int x1, int y1, int x2, int y2) {
  bresenhamline(x1, y1, x2, y2);
}
/**
 * Saves the canvas as a ppm image
 */
void Canvas::imwrite(std::string filename) {
  this->draw();
  std::fstream fs (filename, std::fstream::out);
  if (!fs) 
    std::cout << "Error loading file\n";
  fs << "P3\n" << _w << " " << _h << "\n" << "255\n" ;
  for ( int i = 0; i < _w*_h*3; ++i )
    fs << (int) _pixels[i] << "\n";
  fs.close();
}
/**
 * Bresenham Algorithm to draw lines
 */
void Canvas::bresenhamline(int x1, int y1, int x2, int y2) {
  //@TODO verify conitions of m (currently it works only for 0 <= m <= 1)
  int m_new = 2 * (y2 - y1);
  int slope_error_new = m_new - (x2 - x1);
  Color RED = Color(255,0,0);
  for (int x = x1, y = y1; x <= x2; ++x) {
    PRINTPXL(x,y,RED);
    slope_error_new += m_new;
    if (slope_error_new >= 0) {
      ++y;
      slope_error_new -= 2 * (x2 - x1);
    }
  }
}
/**
 * Mid point algorithm to draw ellipse
 */
void Canvas::midptellipse(int rx, int ry, int xc, int yc) {
  float dx, dy, d1, d2, x, y;
  x = 0;
  y = ry;
  Color GREEN = Color(0,255,0);
  // Initial decision parameter of region 1
  d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
  dx = 2 * ry * ry * x;
  dy = 2 * rx * rx * y;
  // For region 1
  while (dx < dy) {
    // Print points based on 4-way symmetry
    PRINTPXL((int) x + xc,(int) y + yc, GREEN );
    PRINTPXL((int) -x + xc,(int) y + yc, GREEN);
    PRINTPXL((int) x + xc, (int)-y + yc, GREEN);
    PRINTPXL((int) -x + xc, (int)-y + yc, GREEN);

    // Checking and updating value of
    // decision parameter based on algorithm
    if (d1 < 0) {
      x++;
      dx = dx + (2 * ry * ry);
      d1 = d1 + dx + (ry * ry);
    }
    else {
      x++;
      y--;
      dx = dx + (2 * ry * ry);
      dy = dy - (2 * rx * rx);
      d1 = d1 + dx - dy + (ry * ry);
    }
  }
  // Decision parameter of region 2
  d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
       ((rx * rx) * ((y - 1) * (y - 1))) -
        (rx * rx * ry * ry);
  // Plotting points of region 2
  while (y >= 0) {
    PRINTPXL((int) x + xc,(int) y + yc, GREEN );
    PRINTPXL((int) -x + xc,(int) y + yc, GREEN);
    PRINTPXL((int) x + xc,(int) -y + yc, GREEN);
    PRINTPXL((int) -x + xc,(int) -y + yc, GREEN);
    // Checking and updating parameter
    // value based on algorithm
    if (d2 > 0) {
      y--;
      dy = dy - (2 * rx * rx);
      d2 = d2 + (rx * rx) - dy;
    }
    else {
      y--;
      x++;
      dx = dx + (2 * ry * ry);
      dy = dy - (2 * rx * rx);
      d2 = d2 + dx - dy + (rx * rx);
    }
  }
}
/**
 * Fill algorithm scanline
 * \param lines a list o coordenates
 */
void Canvas::scanline(int **lines)
{
  //@TODO ordenar as linhas por y_min
  // - ideia: criar uma classe util de fraction para facilitar os
  // calculos com o coeficiente angular
}

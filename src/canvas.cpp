#include "canvas.hpp"

void Canvas::draw()
{
  // @TODO Call draw function in a vector of drawable entities?
  for ( int i = 0; i < _w*_h*3; ++i )
  {
    _pixels[i] = 150;
  }
  int white[3] = {255,0,0};
  for ( int i = 0; i < 50; ++i )
  {
    PRINTPXL(10+i,10,white);
  }
}

void Canvas::imwrite(std::string filename)
{
  this->draw();
  std::fstream fs (filename, std::fstream::out);
  if (!fs) {
    std::cout << "Error loading file\n";
  }
  fs << "P3\n" << _w << " " << _h << "\n" << "255\n" ;
  for ( int i = 0; i < _w*_h*3; ++i )
  {
    fs << (int) _pixels[i] << "\n";
  }
  fs.close();
}

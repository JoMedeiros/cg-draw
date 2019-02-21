#include "canvas.hpp"

void Canvas::draw()
{
  // @TODO
}

void Canvas::imwrite(std::string filnename)
{
  std::ofstream file;
  file.open(finename);
  file << "P3\n" << _w << " " << _h << "\n" << "255\n" ;
  for ( auto p : _pixels )
  {
    file << p << "\n";
  }
}

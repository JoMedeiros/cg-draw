#include <string>

class Canvas
{
 privante:
   int _w, _h;
   unsigned char * _pixels;
   void scanline();
 public:
   Canvas(int width, int height) : _w{width}, _h{heightr},
     _pixels{new [width*height*3]}
   void draw();
   void imwrite(std::string filename);
   void rect(int widht, int height, int cx=0, int cy=0);
   void line(int x1, int y1, int x2, int y2);
   void poly(int * coords);
};

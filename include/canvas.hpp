

class Canvas
{
 privante:
   int _w, _h;
   unsigned char * _pixels;
 public:
   Canvas(int width, int height) : _w{width}, _h{heightr},
     _pixels{new [width*height*3]}
   void draw();
};

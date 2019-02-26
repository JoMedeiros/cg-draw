#ifndef __UTILS_HPP__
#define __UTILS_HPP__

struct line
{
  int x1,y1,x2,y2;
  int y_min()
  {
    return y1 > y2 ? y1 : y2;
  }
};

#endif// __UTILS_HPP__

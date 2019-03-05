#include <iostream>
#include <map>
#include "yaml-cpp/yaml.h"
#include "canvas.hpp"

int main(int argc, char * argv[])
{
  if (argc == 1) {
    std::cout << "Missing arguments. Usage:\n" << argv[0] <<
      " <image>\n<image>: Yaml image\n";
    return 1;
  }
  int width, height;
  std::map<std::string, Color> color_table;
  YAML::Node img, colors, objects;
  try {
    img = YAML::LoadFile(argv[1]);
    width = img["width"].as<int>();
    height = img["height"].as<int>();
    colors = img["colors"].as<YAML::Node>();
    objects = img["objects"].as<YAML::Node>();
  }
  catch (std::exception & e) {
    std::cout << "Error loading config file:\n";
    std::cout << e.what() << "\n";
  }
  // @TODO Manipulate colors
  // Initiating table of colors
  for(YAML::const_iterator it=colors.begin();it!=colors.end();++it) {
    std::string key = it->first.as<std::string>();
    Color values(0,0,0);
    unsigned char c[3];
    auto node = it->second.as<YAML::Node>();
    for (int i = 0; i < node.size(); ++i)
      c[i] = node[i].as<int>();
    color_table.insert( std::pair<std::string, Color>(key, values) );
  }
  Canvas c(width, height);
  // Initiating objetcts
  for(YAML::const_iterator it=objects.begin();it!=objects.end();++it) {
    if (it->first.as<std::string>() == "line"){
      try {
        auto line = it->second.as<YAML::Node>();
        int x1 = line["x1"].as<int>();
        int x2 = line["x2"].as<int>();
        int y1 = line["y1"].as<int>();
        int y2 = line["y2"].as<int>();
        c.line(x1, y1, x2, y2);
      }
      catch (std::exception & e) {
        std::cout << "Error drawing line.\n"
          << "One of the arguments might be in invalid format.\n"
          << "Remember to put a space after color (: )";
      }
    }
    else if (it->first.as<std::string>() == "rect"){
      try {
        auto line = it->second.as<YAML::Node>();
        int x1 = line["x1"].as<int>();
        int x2 = line["x2"].as<int>();
        int y1 = line["y1"].as<int>();
        int y2 = line["y2"].as<int>();
        c.rect(Point(x1, y1), Point(x2, y2));
      }
      catch (std::exception & e) {
        std::cout << "Error drawing rectangle.\n"
          << "One of the arguments might be in invalid format.\n"
          << "Remember to put a space after color (: )";
      }
    }
    else if (it->first.as<std::string>() == "circle"){
      try {
        YAML::Node circle = it->second.as<YAML::Node>();
        int cx = circle["cx"].as<int>();
        int cy = circle["cy"].as<int>();
        int r = circle["r"].as<int>();
        //if (circle["coisa"])
          //std::cout << "certo\n";
          //for (int i = 0; i < test.size(); ++i)
          //std::cout << "Hey!\n" << test[i].as<int>();
        c.circle(Point(cx, cy), r);
      }
      catch (std::exception & e) {
        std::cout << "Error drawing circle.\n"
          << "One of the arguments might be in invalid format.\n"
          << "Remember to put a space after color (: )";
      }
    }
  }

  Point pt1(10, 10), pt2(10, 50), pt3(20, 100),
        pt4(400, 400);

  // A line with -1 < m < 0
  //c.line(pt1, pt2, Color(0,255,150));
  // A line with 0 < m < 1
  //c.line(pt2, pt3, Color(255,0,0));
  // A line with m > 1
  //c.line(pt3, pt4);
  // A line with m < -1
  //c.line(pt4, pt1);
  // A circle
  c.circle(pt4, 200, Color(0,0,255));
  // A rectangle
  c.rect(200,50, Point(150,50), Color(255,255,0));
  std::vector<Point> pts = {pt1, pt2, pt3, pt4};
  c.poly(pts, Color(255,111,97));
  // Saving image
  c.imwrite(std::string("img.ppm"));
  std::cout << "Hello YAML!\n";
  return 0;
}

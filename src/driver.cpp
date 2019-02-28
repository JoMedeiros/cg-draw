//#include "yaml-cpp/yaml.h"
#include <iostream>
#include "canvas.hpp"

int main(int argc, char * argv[])
{
  /*try {
    YAML::Node config = YAML::LoadFile("config.yaml");
  }
  catch (std::exception & e) {
    std::cout << "Error loading config file:\n";
    std::cout << e.what() << "\n";
  }*/
  Canvas c(400,300);
  // A line with -1 < m < 0
  c.line(10,100, 200, 50);
  // A line with 0 < m < 1
  c.line(20,20,200,100);
  // A line with m > 1
  c.line(250,50,300,200);
  // A line with m < -1
  c.line(200,250,250,50);
  c.imwrite(std::string("img.ppm"));
  std::cout << "Hello YAML!\n";
  return 0;
}

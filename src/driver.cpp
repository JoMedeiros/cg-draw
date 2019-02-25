#include "yaml-cpp/yaml.h"
#include <iostream>
#include "canvas.hpp"

int main(int argc, char * argv[])
{
  try {
    YAML::Node config = YAML::LoadFile("config.yaml");
  }
  catch (std::exception & e) {
    std::cout << "Error loading config file:\n";
    std::cout << e.what() << "\n";
  }
  Canvas c(400,300);
  c.imwrite(std::string("img.ppm"));
  std::cout << "Hello YAML!\n";
  return 0;
}

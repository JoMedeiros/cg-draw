#include <iostream>
#include <map>
#include "yaml-cpp/yaml.h"
#include "canvas.hpp"

int main(int argc, char * argv[])
{
  if (argc == 1) {
    std::cout << "Missing arguments. Usage:\n" << argv[0] <<
      " <inpu_image> [<output_image>]\n<input_image>: Yaml description image"
      <<"\n<output_image>: output ppm image file\n";
    return 1;
  }
  else if (argc > 3) {
    std::cout << "Too much arguments. Usage:\n" << argv[0] <<
      " <inpu_image> [<output_image>]\n<input_image>: Yaml description image"
      <<"\n<output_image>: output ppm image file\n";
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
    return -1;
  }
  
  Canvas c(width, height);
  
  // @TODO Manipulate colors
  // Initiating table of colors
  for(YAML::const_iterator it=colors.begin();it!=colors.end();++it) {
    std::string key = it->first.as<std::string>();
    auto node = it->second.as<YAML::Node>();
    Color values; values.r = node[0].as<int>(); values.g = node[1].as<int>();
    values.b = node[2].as<int>();
    color_table.insert( make_pair( key, values ) );
  }
  
  // Initiating objetcts
  for(YAML::const_iterator it=objects.begin();it!=objects.end();++it) {
    if (it->first.as<std::string>() == "line"){
      try {
        auto line = it->second.as<YAML::Node>();
        int x1 = line["x1"].as<int>(); int x2 = line["x2"].as<int>();
        int y1 = line["y1"].as<int>(); int y2 = line["y2"].as<int>();
        auto color = line["stroke"]; Color stroke;
        if (color.Type() == YAML::NodeType::Scalar)
          stroke = color_table[color.as<string>()];
        c.line(x1, y1, x2, y2, stroke);
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
    else if (it->first.as<string>() == "circle") {
      try {
        YAML::Node circle = it->second.as<YAML::Node>();
        int cx = circle["cx"].as<int>();
        int cy = circle["cy"].as<int>();
        int r = circle["r"].as<int>();
        auto stroke_node = circle["stroke"];
        Color stroke, fill;
        if (stroke_node.Type() == YAML::NodeType::Scalar)
          stroke = color_table[stroke_node.as<string>()];
        else if (stroke_node.Type() == YAML::NodeType::Sequence)
          stroke = Color(stroke_node[0].as<int>(), stroke_node[1].as<int>(),
              stroke_node[2].as<int>());
        //if (circle["coisa"])
          //std::cout << "certo\n";
          //for (int i = 0; i < test.size(); ++i)
          //std::cout << "Hey!\n" << test[i].as<int>();
        c.circle(Point(cx, cy), r, stroke);
      }
      catch (std::exception & e) {
        std::cout << "Error drawing circle.\n"
          << "One of the arguments might be in invalid format.\n"
          << "Remember to put a space after color (: )";
      }
    }
    else if (it->first.as<string>() == "polygon") {
      try {
        YAML::Node polygon = it->second.as<YAML::Node>();
        auto points_node = polygon["points"];
        vector<Point> points;
        for (int i=0; i < points_node.size(); i+=2){
          Point pt(points_node[i].as<int>(), points_node[i+1].as<int>());
          points.push_back(pt);
        }
        c.polygon(points, Color(255,0,255));
          //, Color(255,0,0));
      }
      catch (exception & e) {
        std::cout << "Error drawing circle.\n"
          << "One of the arguments might be in invalid format.\n"
          << "Remember to put a space after color (: )";
      }
    }
  }

  //Point pt1(410, 210), pt2(150, 150), pt3(120, 200),
        //pt4(400, 40);

  //c.circle(pt4, 200, Color(0,0,255));
  // A rectangle
  //c.rect(200,50, Point(150,50), Color(255,255,0));
  //std::vector<Point> pts = {pt1, pt2, pt3, pt4};
  //c.polygon(pts, Color(255,111,97));
  //c.scanline(pts);
  
  // Saving image
  std::string ofile("img.ppm");
  if ( argc == 3 ) {
    std::string temp(argv[2]);
    if ( temp.size() > 4 ) {
      if ( temp.substr(temp.size()-4, 4).compare(".ppm") == 0 )
        ofile = temp;
      else 
        ofile = temp + ".ppm";
    }
  }
  c.imwrite(ofile);
  std::cout << "Hello YAML!\n";
  return 0;
}

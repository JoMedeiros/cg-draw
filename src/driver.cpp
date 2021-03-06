/**
 * @file    driver.cpp
 * @brief   Program to load yaml and generate an output image using Canvas 
 * class.
 *
 * @author  Josivan Medeiros da Silva Gois
 * @date    18/03/2019
 */
#include <iostream>
#include <map>
#include "yaml-cpp/yaml.h"
#include "canvas.hpp"

std::map<std::string, Color> color_table;
Color load_color(YAML::Node & color_node) {
  Color color;
  if (color_node.Type() == YAML::NodeType::Scalar)
    color = color_table[color_node.as<string>()];
  else if (color_node.Type() == YAML::NodeType::Sequence)
    color = Color(color_node[0].as<int>(), color_node[1].as<int>(),
        color_node[2].as<int>());
  return color;
}
int load_algorithm(YAML::Node & alg_node) {
  string alg = alg_node.as<string>();
  if (alg.compare("scanline")) return ALG::SCANLINE;
  else if (alg.compare("dda")) return ALG::DDA;
  else if (alg.compare("bresenham")) return ALG::BRESENHAM;
  else if (alg.compare("midpoint")) return ALG::MIDPOINT;
  else return ALG::DDA;
}
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
    Color values; 
    values.r = node[0].as<int>(); 
    values.g = node[1].as<int>();
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
        auto stroke_node = line["stroke"]; Color stroke;
        if (stroke_node.Type() == YAML::NodeType::Scalar)
          stroke = color_table[stroke_node.as<string>()];
        else if (stroke_node.Type() == YAML::NodeType::Sequence and stroke_node.size() == 3) {
          stroke.r = (unsigned char) stroke_node[0].as<int>();
          stroke.g = (unsigned char) stroke_node[1].as<int>();
          stroke.b = (unsigned char) stroke_node[2].as<int>();
        }
        c.line(x1, y1, x2, y2, stroke, ALG::MIDPOINT);
      }
      catch (std::exception & e) {
        std::cout << "Error drawing line.\n"
          << "One or more of the arguments might be in invalid format.\n"
          << e.what() << "\n";
      }
    }
    else if (it->first.as<string>() == "circle") {
      try {
        YAML::Node circle = it->second.as<YAML::Node>();
        int cx = circle["cx"].as<int>();
        int cy = circle["cy"].as<int>();
        int r = circle["r"].as<int>();
        auto stroke_node = circle["stroke"];
        auto fill_node = circle["fill"];
        Color stroke = load_color(stroke_node), fill;
        if (fill_node.Type() != YAML::NodeType::Null and 
            fill_node.Type() != YAML::NodeType::Undefined) {
          fill = load_color(fill_node);
          c.circle(Point(cx, cy), r, stroke, fill);
        }
        else
          c.circle(Point(cx, cy), r, stroke);
      }
      catch (std::exception & e) {
        std::cout << "Error drawing circle.\n"
          << "One of the arguments might be in invalid format.\n"
          << e.what() << "\n";
      }
    }
    else if (it->first.as<string>() == "polyline") {
      try {
        YAML::Node polyline = it->second.as<YAML::Node>();
        auto points_node = polyline["points"];
        vector<Point> points;
        for (int i=0; i < points_node.size(); i+=2){
          Point pt(points_node[i].as<int>(), points_node[i+1].as<int>());
          points.push_back(pt);
        }
        auto stroke_node = polyline["stroke"];
        Color stroke = load_color(stroke_node);
        c.polyline(points, stroke);
      }
      catch (exception & e) {
        std::cout << "Error drawing circle.\n"
          << "One of the arguments might be in invalid format.\n"
          << e.what() << "\n";
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
        auto stroke_node = polygon["stroke"];
        auto fill_node = polygon["fill"];
        auto alpha_node = polygon["alpha"];
        Color stroke = load_color(stroke_node), fill;
        if (fill_node.Type() != YAML::NodeType::Null and
            fill_node.Type() != YAML::NodeType::Undefined) {
          fill = load_color(fill_node);
          if (alpha_node.Type() != YAML::NodeType::Null and 
            alpha_node.Type() != YAML::NodeType::Undefined ) {
            float alpha = alpha_node.as<float>();
            c.polygon(points, stroke, fill, alpha);
          }
          else {
            c.polygon(points, stroke, fill);
          }
        }
        else
          c.polygon(points, stroke);
      }
      catch (exception & e) {
        std::cout << "Error drawing polyline.\n"
          << "One of the arguments might be in invalid format.\n"
          << e.what() << "\n";
      }
    }
    if (it->first.as<std::string>() == "arc"){
      try {
        auto arc = it->second.as<YAML::Node>();
        int cx = arc["cx"].as<int>(); 
        int startX = arc["startX"].as<int>();
        int cy = arc["cy"].as<int>();
        int startY = arc["startY"].as<int>();
        int angle = arc["angle"].as<float>();
        auto stroke_node = arc["stroke"];
        Color stroke = load_color( stroke_node );
        cout << "angle: " << angle << "\n";
        c.arc( Point( cx, cy) , Point( startX, startY ), angle, stroke );
      }
      catch (std::exception & e) {
        std::cout << "Error drawing arc.\n"
          << "One or more of the arguments might be in invalid format.\n"
          << e.what() << "\n";
      }
    }
  }

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
  return 0;
}

#!/bin/bash

git clone https://github.com/jbeder/yaml-cpp.git
mkdir yaml-cpp/build
cd yaml-cpp/build
cmake ../ -DCMAKE_INSTALL_PREFIX=../../
make
make install
sudo rm -r yaml-cpp/

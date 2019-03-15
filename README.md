# Rasterinha

Rasterinha is a Computer Graphics project: a 2D rasterizer.

## Requirements

- git
- yaml-cpp

## Usage

You will need to install [yaml-cpp](https://github.com/jbeder/yaml-cpp/)
library. Run the command:

``` 
bash ./install-yaml-cpp-locally.sh 
```

If you already have the `yaml-cpp` installed you will need to modify
the `CMakeLists.txt` to link to your library and include files. 
To compile:

```
git clone https://github.com/JoMedeiros/cg-draw.git
mkdir build && cd build
cmake ..
make
```

Run the program with:

```
./cg-draw <image_name> 
```

Where \<image\_name\> is a path to a image descriptor in yaml format.

### yaml image format

Here's an example of image description:

```
width: 800
height: 600
colors:
  white: [255,255,255]
objects:
  line: {x1: 100, y1: 100, x2: 300, y2: 300}
  circle: {cx: 400, cy: 300, r:200}
```

First we have `width` and `height` of the image in pixels.
Then we have a table of colors. Each colors is described as an array
of three integers. Finally the objects to be drawn. Each object can be:

- line: The arguments are `x1` and `y1` the initial point and `x2` and 
`y2` the end point.
- circle: The arguments are `cx` and `cy` the center point and `r` the 
radius.

## Third party libraries

- [yaml-cpp](https://github.com/jbeder/yaml-cpp/):
a library to parse yaml files


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

## yaml image format

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
Then we have a table of colors. Each color in `colors` is described as an array
of three integers. Finally the objects to be drawn. Each object can be:

#### Line 
The arguments are `x1` and `y1` the initial point and `x2` and `y2` the end point.
`stroke` is the color (defined in `colors` table or in format `[r,g,b]`) of the line.

#### Circle 
The arguments are `cx` and `cy` the center point and `r` the radius.
`stroke` is the color (defined in `colors` table or in format `[r,g,b]`) of the line.

#### Polyline
```
  polyline: {points: [100, 10, 
                      40, 198, 
                      190, 78, 
                      10, 78, 
                      160, 198], stroke: my_green}
```

`points` is a list of points in order to be connected.

`stroke` is the color (defined in `colors` table or in format `[r,g,b]`) of the line.

#### Polygon
```
  polyline: {points: [100, 10, 
                      40, 198, 
                      190, 78, 
                      10, 78, 
                      160, 198], stroke: my_green}
```

`points` is a list of points in order to be connected (it will close the polygon
connecting the last and the first points).

`stroke` is the color (defined in `colors` table or in format `[r,g,b]`) of the line.

`fill` is the color (defined in `colors` table or in format `[r,g,b]`) to fill the 
polygon.

`alpha` is the fill transparancy.

## Third party libraries

- [yaml-cpp](https://github.com/jbeder/yaml-cpp/):
a library to parse yaml files


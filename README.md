# Computer Graphics - 2D draw

## Requisites

- git
- yaml-cpp

## Usage

./cg-draw \<imagem\_name\>

### yaml image format

Here's an example of image description
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



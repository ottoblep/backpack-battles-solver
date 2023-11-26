#include <array>
#include <iostream>

using std::array;

typedef array<int, 2> coord;

void printCoords(coord coords){
    std::cout << coords[0] << " " << coords[1] << "\n";
}

coord rotateCoords(coord coords, int rotation){
  switch (rotation){
    case 1:
      coords[0] = coords[1];
      coords[1] = -coords[0];
      break;
    case 2:
      coords[0] = -coords[0];
      coords[1] = -coords[1];
      break;
    case 3:
      coords[0] = -coords[1];
      coords[1] = coords[0];
      break;
  }
  return coords;
}

coord addCoords(coord coords1, coord coords2){
    coord result;
    result[0] = coords1[0] + coords2[0];
    result[1] = coords1[1] + coords2[1];
    return result;
}
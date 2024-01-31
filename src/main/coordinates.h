#include <array>
#include <iostream>

#define GRID_SIZE_X 9
#define GRID_SIZE_Y 7
// Search space size for 3 bags and 5 items 
// (9 * 7 * 4)^8 = 16e18  
// Singlecore performance 500ns/configuration = 250 thousand years
// Multicore performance x8 ~60ns/configuration = 30 thousand years
using std::array;

typedef array<int, 2> coord;

// This represents a matrix the size of the maximum grid possible
typedef array<array<int, GRID_SIZE_Y>, GRID_SIZE_X> gridmatrix;

// Checks the outside boundaries of the grid
bool isValidGridPosition(coord coords) {
  if (coords[0] < 0 || coords[1] < 0 || coords[0] > GRID_SIZE_X-1|| coords[1] > GRID_SIZE_Y-1) { return false; }
  else { return true; }
}

void printCoords(coord coords){
    std::cout << coords[0] << " " << coords[1] << "\n";
}

// This prints the placement matrix as follows:
//  - Empty space (0 in matrix) is not printed
//  - Bag space (-1 in matrix) is printed as 0 
//  - Items (>0 in matrix) are printed as >0 
void printGridMatrix(gridmatrix matrix){
  for (int i = 0; i<GRID_SIZE_Y; i++) {
    for (int k = 0; k<GRID_SIZE_X; k++) {
      if (matrix[k][i]>0)
        std::cout << matrix[k][i] << " ";
      else if (matrix[k][i]==0)
        std::cout << "  ";
      else
        std::cout << "0 ";
    }
    std::cout << "\n";
  }
}

coord rotateCoords(coord coords, int rotation){
  coord old_coords = coords;
  switch (rotation){
    case 1:
      coords[0] = old_coords[1];
      coords[1] = -old_coords[0];
      break;
    case 2:
      coords[0] = -old_coords[0];
      coords[1] = -old_coords[1];
      break;
    case 3:
      coords[0] = -old_coords[1];
      coords[1] = old_coords[0];
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
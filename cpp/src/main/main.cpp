#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <experimental/optional>

#include "items.h"
#include "bags.h"
#include "coordinates.h"

using std::vector;
using std::array;
using std::experimental::optional;

#define GRID_SIZE_X 9
#define GRID_SIZE_Y 7

// This represents a matrix the size of the maximum grid possible
typedef array<array<int, GRID_SIZE_Y>, GRID_SIZE_X> gridmatrix;

void printGridMatrix(gridmatrix matrix){
  for (int i = 0; i<GRID_SIZE_Y; i++) {
    for (int k = 0; k<GRID_SIZE_X; k++) {
      std::cout << matrix[k][i] << " ";
    }
    std::cout << "\n";
  }
}

// This assigns random coordinates and rotation to each item.
std::tuple<vector<Bag>, vector<Item>> randomPlacement(vector<Bag> baglist, vector<Item> itemlist){
  for (int i = 0;i<baglist.size();i++){
    baglist[i].position = {std::rand()%GRID_SIZE_X, std::rand()%GRID_SIZE_Y};
    baglist[i].rotation = std::rand()%4;
  }

  for (int i = 0;i<itemlist.size();i++){
    itemlist[i].position = {std::rand()%GRID_SIZE_X, std::rand()%GRID_SIZE_Y};
    itemlist[i].rotation = std::rand()%4;
  }

  return std::make_pair(baglist, itemlist);
}

// This iterates through the item list and attempts to place all of them in the placement matrix
optional<gridmatrix> generatePlacementMatrix(vector<Bag> baglist, vector<Item> itemlist){
  gridmatrix result = {};
  coord new_block;

  // Place the bags
  for (int i = 0;i<baglist.size();i++){
    for (int block_num = 0; block_num<baglist[i].blocks.size(); block_num++){
      new_block = addCoords(rotateCoords(baglist[i].blocks[block_num], baglist[i].rotation), baglist[i].position);

      // Check for invalid block positions
      if (new_block[0] < 0 || new_block[1] < 0 ||
        new_block[0] > GRID_SIZE_X-1 || new_block[1] > GRID_SIZE_Y-1) {
        return {};
      }

      // Bags can not be placed on another bag 
      if (result[new_block[0]][new_block[1]] == 0){
        // Mark the location of an item with its number in the baglist
        result[new_block[0]][new_block[1]] = -1;
      } else {
        return {};
      }
    }
  }
  // Place the items in the bags
  for (int i = 0;i<itemlist.size();i++){
    for (int block_num = 0; block_num<itemlist[i].blocks.size(); block_num++){
      new_block = addCoords(rotateCoords(itemlist[i].blocks[block_num], itemlist[i].rotation), itemlist[i].position);

      // Check for invalid block positions
      if (new_block[0] < 0 || new_block[1] < 0 ||
        new_block[0] > GRID_SIZE_X-1|| new_block[1] > GRID_SIZE_Y-1) {
        return {};
      }

      // Items can only be placed in a bag 
      if (result[new_block[0]][new_block[1]] == -1){
        // Mark the location of an item with its number in the baglist
        result[new_block[0]][new_block[1]] = i+1;
      } else {
        return {};
      }
    }
  }
  return result;
}

int main(int argc, char** argv) {

  std::srand(std::time(nullptr)); // use current time as seed for random generator

  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag")};
  vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan")};

  gridmatrix best_configuration = {};
  for (int i = 0; i<1e6; i++) {
    std::tie(baglist, itemlist) = randomPlacement(baglist, itemlist);
    // Check validity of configuration
    optional<gridmatrix> placement_matrix_result = generatePlacementMatrix(baglist, itemlist);
    best_configuration = placement_matrix_result.value_or(best_configuration);
  } 

  // If no valid configuration is found this will be all zeros
  printGridMatrix(best_configuration);

  return EXIT_SUCCESS;
}

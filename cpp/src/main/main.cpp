#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <optional>

#include "items.h"
#include "bags.h"
#include "coordinates.h"

using std::vector;
using std::array;
using std::optional;

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

bool isValidGridPosition(coord coords) {
  if (coords[0] < 0 || coords[1] < 0 || coords[0] > GRID_SIZE_X-1|| coords[1] > GRID_SIZE_Y-1) { return false; }
  else { return true; }
}

// This assigns random coordinates and rotation to each item.
std::tuple<vector<Bag>, vector<Item>> generateRandomConfiguration(vector<Bag> baglist, vector<Item> itemlist){
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
// This checks if a configuration is in the valid search space
optional<gridmatrix> generatePlacementMatrix(vector<Bag> baglist, vector<Item> itemlist){
  gridmatrix result = {};
  coord new_block;

  // Place the bags
  for (int i = 0;i<baglist.size();i++){
    for (int block_num = 0; block_num<baglist[i].blocks.size(); block_num++){
      new_block = addCoords(rotateCoords(baglist[i].blocks[block_num], baglist[i].rotation), baglist[i].position);

      if (!isValidGridPosition(new_block)) return {};

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

      if (!isValidGridPosition(new_block)) return {};

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

// Counts valid connections between items
// This is the objective function
int countValidConnections(gridmatrix placement_matrix, vector<Item> itemlist) {
  int score = 0;
  coord conn_position;
  for (int item_idx = 0; item_idx<itemlist.size(); item_idx++) {
      // TODO: Check if item types are compatible for connection
      // TODO: Certain items can connect anywhere 
      // Each item can connect at n specific points
      for (int conn_block_idx = 0; conn_block_idx < itemlist[item_idx].connection_blocks.size(); conn_block_idx++) {
        conn_position = addCoords(rotateCoords(itemlist[item_idx].connection_blocks[conn_block_idx], itemlist[item_idx].rotation), itemlist[item_idx].position);

        if (!isValidGridPosition(conn_position)) continue;

        // Increase score if there is a block at the connection point 
        if (placement_matrix[conn_position[0]][conn_position[1]] > 0)
          score++;
      }
  }
  return score;
}

gridmatrix randomSearchStrategy(vector<Bag> baglist, vector<Item>itemlist, int maxRetries) {
  gridmatrix best_configuration = {};
  int best_score = 0;
  int abort_counter = 0;
  int new_score = 0;

  while (abort_counter < maxRetries) {
    std::tie(baglist, itemlist) = generateRandomConfiguration(baglist, itemlist);
    // Check validity of configuration
    optional<gridmatrix> placement_matrix_result = generatePlacementMatrix(baglist, itemlist);

    // Evaluate score of configuration
    if (placement_matrix_result.has_value()) {
      new_score = countValidConnections(placement_matrix_result.value(),itemlist);
      if (new_score > best_score) {
        best_configuration = placement_matrix_result.value_or(best_configuration);
        best_score = new_score;
        std::cout << "New best configuration score: " << best_score << "\n";
        abort_counter = 0;
      }
    } else {
      abort_counter++;
    }
  }
  return best_configuration;
}

int main(int argc, char** argv) {
  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // Define item and bag inventory
  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag")};
  vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan"), LookupItem("GlovesOfHaste")};

  // Choose a search strategy
  gridmatrix best_configuration = randomSearchStrategy(baglist, itemlist, 1e6);

  // If no valid configuration is found this will be all zeros
  printGridMatrix(best_configuration);

  return EXIT_SUCCESS;
}

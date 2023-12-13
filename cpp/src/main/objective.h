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

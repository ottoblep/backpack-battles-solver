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
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>
#include <ctime>

#include "items.h"
#include "bags.h"
#include "coordinates.h"

using std::vector;
using std::array;
using std::optional;

#define GRID_SIZE_X 9
#define GRID_SIZE_Y 7
// Search space size for 3 bags and 5 items 
// (9 * 7 * 4)^8 = 16e18  
// Singlecore performance 500ns/configuration = 250 thousand years
// Multicore performance x8 ~60ns/configuration = 30 thousand years

// This represents a matrix the size of the maximum grid possible
typedef array<array<int, GRID_SIZE_Y>, GRID_SIZE_X> gridmatrix;

std::mutex global_configuration_mutex;
int global_best_score;
gridmatrix global_best_configuration;

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

// Checks the outside boundaries of the grid
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

// Independently generates and evaluates configurations for a fixed time
void randomSearchThread(vector<Bag> baglist, vector<Item>itemlist, int runtime) {
  std::chrono::time_point<std::chrono::high_resolution_clock> t_start = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock> t_end;
  int new_score;
  optional<gridmatrix> placement_matrix_result;

  while ( std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count() < runtime ) {
    std::tie(baglist, itemlist) = generateRandomConfiguration(baglist, itemlist);
    // Check validity of configuration
    placement_matrix_result = generatePlacementMatrix(baglist, itemlist);

    // Evaluate score of configuration
    if (placement_matrix_result.has_value()) {
      new_score = countValidConnections(placement_matrix_result.value(),itemlist);
      if (new_score > global_best_score) {
        global_configuration_mutex.lock();
        global_best_configuration = placement_matrix_result.value();
        global_best_score = new_score;
        global_configuration_mutex.unlock();
        std::cout << "New best configuration score: " << global_best_score << "\n";
      }
    }
    t_end = std::chrono::high_resolution_clock::now();
  }
}

// Launches multiple random search threads which write their result to global_best_configuration 
void randomSearchStrategy(vector<Bag> baglist, vector<Item>itemlist, int runtime, int threads) {
  std::vector<std::thread> threadlist;
  std::cout << "Spawning " << threads << " search threads running for " << runtime << " seconds.\n";

  for (int t = 0; t < threads; t++) {
    threadlist.push_back(std::thread(randomSearchThread, baglist, itemlist, runtime));
  }
  for (int t = 0; t < threads; t++) {
    threadlist[t].join();
  }
  std::cout << "All threads have exited.\n";
}

int main(int argc, char** argv) {
  // TODO: Add argparser to choose strategy and parameters instead of hard-coding
  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // Define item and bag inventory
  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag"), LookupBag("StaminaSack"),};
  vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan"), LookupItem("GlovesOfHaste")};

  // Choose a search strategy
  randomSearchStrategy(baglist, itemlist, 60, 6);

  // If no valid configuration is found this will be all zeros
  printGridMatrix(global_best_configuration);

  return EXIT_SUCCESS;
}

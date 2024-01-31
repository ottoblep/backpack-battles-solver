#include <thread>
#include <memory>
#include <chrono>
#include <ctime>
#include <optional>
#include <vector>
#include <cassert>

#ifndef objective
#include "objective.cpp"
#define objective
#endif
#ifndef thread_handling
#include "threadHandling.h"
#define thread_handling
#endif

using std::vector;
using std::optional;

// Finds either largest bag or item in list
template <typename T>
int findLargest(T list) {
  // Select bag with the most space 
  int largest_idx = 0;
  int largest_size = 0; 
  int size;
  for (int i = 0; i<list.size(); i++){
    size = list[i].blocks.size();
    if (size > largest_size){
      largest_idx = i;
      largest_size = size;
    }
  }
  assert(largest_idx >= 0); assert(largest_size > 0);
  return largest_idx;
}

// Greedily places bags and objects iteratively
std::tuple<vector<Bag>, vector<Item>> generateValidConfigurationGreedy(vector<Bag> baglist, vector<Item> itemlist){
  vector<Bag> original_baglist = baglist;
  vector<Item> original_itemlist = itemlist;
  vector<Bag> test_baglist;
  vector<Item> test_itemlist;

  bool done = false;
  int placement_tries;
  int largest_bag_idx;

  // Place bags from largest to smallest
  for (int i = 0; i < original_baglist.size(); i++){
    // Find largest remaining bag
    largest_bag_idx = findLargest<vector<Bag>>(baglist);

    // Move it to the working list
    test_baglist.push_back(baglist[largest_bag_idx]);
    baglist.erase(baglist.begin() + largest_bag_idx);

    // Try until placing that bag
    placement_tries = 0;
    do {
      placement_tries++;
      test_baglist.back().position = {std::rand()%GRID_SIZE_X, std::rand()%GRID_SIZE_Y};
      test_baglist.back().rotation = std::rand()%4;

      // Abort if there is no way to place the part
      if (placement_tries > 1e5) {
        return {};
      }
    } while (!generateBagMatrix(test_baglist).has_value());
  }
  optional<gridmatrix> bag_matrix_result = generateBagMatrix(test_baglist).value();
  assert(bag_matrix_result.has_value());
  gridmatrix bag_matrix = bag_matrix_result.value();
  baglist = test_baglist;

  // Place items from largest to smallest
  for (int i = 0; i < original_itemlist.size(); i++){
    // Find largest remaining bag
    int largest_item_idx = findLargest<vector<Item>>(itemlist);

    // Move it to the working list
    test_itemlist.push_back(itemlist[largest_item_idx]);
    itemlist.erase(itemlist.begin() + largest_item_idx);

    // Try until placing that bag
    placement_tries = 0;
    do {
      placement_tries++;
      test_itemlist.back().position = {std::rand()%GRID_SIZE_X, std::rand()%GRID_SIZE_Y};
      test_itemlist.back().rotation = std::rand()%4;
      // Abort if there is no way to place the part
      if (placement_tries > 1e5) {
        return {};
      }
    } while (!generateItemMatrix(bag_matrix, test_itemlist).has_value());
  }
  gridmatrix item_matrix = generateItemMatrix(bag_matrix, test_itemlist).value();
  itemlist = test_itemlist;

  // TODO: select item with the most connection points
  // TODO: Place first item in center of bag
  // TODO: Place additional items around the first

  return std::make_pair(baglist, itemlist);
}

// Independently generates and evaluates configurations for a fixed time
void greedySearchThread(vector<Bag> baglist, vector<Item>itemlist, int runtime) {
  std::chrono::time_point<std::chrono::high_resolution_clock> t_start = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock> t_end;
  int new_score;
  optional<gridmatrix> placement_matrix_result;

  while ( std::chrono::duration_cast<std::chrono::seconds>(t_end - t_start).count() < runtime ) {

    std::tie(baglist, itemlist) = generateValidConfigurationGreedy(baglist, itemlist);

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
gridmatrix greedySearchStrategy(vector<Bag> baglist, vector<Item>itemlist, int runtime, int threads) {
  std::cout << "Running greedy search strategy.\n";
  std::vector<std::thread> threadlist;
  std::cout << "Spawning " << threads << " search threads running for " << runtime << " seconds.\n";

  for (int t = 0; t < threads; t++) {
    threadlist.push_back(std::thread(greedySearchThread, baglist, itemlist, runtime));
  }
  for (int t = 0; t < threads; t++) {
    threadlist[t].join();
  }
  std::cout << "All threads have exited.\n";
  return global_best_configuration;
}

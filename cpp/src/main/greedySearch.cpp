#include <thread>
#include <memory>
#include <chrono>
#include <ctime>
#include <optional>

#ifndef objective
#include "objective.cpp"
#define objective
#endif

using std::vector;
using std::optional;

#include "threadHandling.h"

int findLargestBag(vector<Bag> baglist) {
    // Select bag with the most space 
    int largest_bag_idx = 0;
    int largest_size = 0; 
    int bagsize;
    for (int i = 0;i<baglist.size();i++){
      bagsize = baglist[i].blocks.size();
      if ( bagsize > largest_size){
        largest_bag_idx = i;
        largest_size = bagsize;
      }
    }
    return largest_bag_idx;
}

// Greedily places bags and objects iteratively
std::tuple<vector<Bag>, vector<Item>> generateValidConfigurationGreedy(vector<Bag> baglist, vector<Item> itemlist){
  vector<Bag> partial_baglist;

  vector<Bag> original_baglist = baglist;
  vector<Item> original_itemlist = itemlist;
  vector<Bag> iteration_baglist = baglist;
  vector<Item> iteration_itemlist = itemlist;

  bool done = false;

  // Try to place each bag sequentally from largest to smallest
  do {
    for (int i = 0;i<baglist.size();i++){
      // Find largest remaining bag
      int largest_bag_idx = findLargestBag(iteration_baglist);
      // Try placing that bag and remove from remaining
      do {
      baglist[i].position = {std::rand()%GRID_SIZE_X, std::rand()%GRID_SIZE_Y};
      baglist[i].rotation = std::rand()%4;
      } while (!generatePlacementMatrix(baglist, itemlist))
      iteration_baglist.erase(iteration_baglist.begin() + largest_bag_idx);
    }
  } while (!done);

    // TODO: Place each item sequentally
    // TODO: select item with the most connection points
    // TODO: Place first item in center of bag
    // TODO: Place additional items around the first
  for (int i = 0;i<itemlist.size();i++){
    itemlist[i].position = {std::rand()%GRID_SIZE_X, std::rand()%GRID_SIZE_Y};
    itemlist[i].rotation = std::rand()%4;
  }

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

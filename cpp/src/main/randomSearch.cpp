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
gridmatrix randomSearchStrategy(vector<Bag> baglist, vector<Item>itemlist, int runtime, int threads) {
  std::vector<std::thread> threadlist;
  std::cout << "Spawning " << threads << " search threads running for " << runtime << " seconds.\n";

  for (int t = 0; t < threads; t++) {
    threadlist.push_back(std::thread(randomSearchThread, baglist, itemlist, runtime));
  }
  for (int t = 0; t < threads; t++) {
    threadlist[t].join();
  }
  std::cout << "All threads have exited.\n";
  return global_best_configuration;
}

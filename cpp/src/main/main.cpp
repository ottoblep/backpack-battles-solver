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

#include "randomSearch.cpp"
#include "greedySearch.cpp"

using std::vector;

int main(int argc, char** argv) {
  // TODO: Add argparser to choose strategy and parameters instead of hard-coding
  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // Define item and bag inventory
  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("PotionBelt"),  LookupBag("RangerBag"), LookupBag("StaminaSack"),};
  vector<Item> itemlist = {LookupItem("Pan"), LookupItem("WoodenSword"), LookupItem("Pan"), LookupItem("GlovesOfHaste"), LookupItem("GlovesOfHaste")};

  // Choose a search strategy
  
  gridmatrix search_result = greedySearchStrategy(baglist, itemlist, 5, 11);

  global_best_score = 0;
  search_result = randomSearchStrategy(baglist, itemlist, 60, 11);

  // If no valid configuration is found this will be all zeros
  printGridMatrix(search_result);

  return EXIT_SUCCESS;
}

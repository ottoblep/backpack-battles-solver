#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>

#include "items.h"
#include "bags.h"

using std::vector;
using std::array;

// This represents a matrix the size of the maximum grid possible
typedef array<array<int, 7>, 9> GridMatrix;

std::tuple<vector<Bag>, vector<Item>> randomPlacement(vector<Bag> baglist, vector<Item> itemlist){
  return std::make_pair(baglist, itemlist);
}

GridMatrix generatePlacementMatrix(vector<Bag> baglist, vector<Item> itemlist){
  GridMatrix result = {};
  return result;
}

int main(int argc, char** argv) {

  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag")};
  vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan")};

  std::tie(baglist, itemlist) = randomPlacement(baglist, itemlist);

  // Check validity of configuration
  GridMatrix placement_matrix = generatePlacementMatrix(baglist, itemlist);

  std::cout << "Test";
  return EXIT_SUCCESS;
}

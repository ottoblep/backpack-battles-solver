#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "items.h"
#include "bags.h"

using std::vector;
using std::array;

// This represents a matrix the size of the maximum grid possible
typedef array<array<int, 7>, 9> GridMatrix;

std::tuple<vector<Bag>, vector<Item>> randomPlacement(vector<Bag> baglist, vector<Item> itemlist){
  for (int i = 0;i<baglist.size();i++){
    baglist[i].position = {std::rand()%9, std::rand()%7};
    baglist[i].rotation = std::rand()%4;
  }
  for (int i = 0;i<itemlist.size();i++){
    itemlist[i].position = {std::rand()%9, std::rand()%7};
    itemlist[i].rotation = std::rand()%4;
  }

  return std::make_pair(baglist, itemlist);
}


GridMatrix generatePlacementMatrix(vector<Bag> baglist, vector<Item> itemlist){
  GridMatrix result = {};
  return result;
}

int main(int argc, char** argv) {
  std::srand(std::time(nullptr)); // use current time as seed for random generator

  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag")};
  vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan")};

  std::tie(baglist, itemlist) = randomPlacement(baglist, itemlist);

  for (Bag bag : baglist) {
    std::cout << bag.position[0] << "\n";
    std::cout << bag.position[1] << "\n";
    std::cout << bag.rotation << "\n";
  }
  for (Item item : itemlist) {
    std::cout << item.position[0] << "\n";
    std::cout << item.position[1] << "\n";
    std::cout << item.rotation << "\n";
  }

  // Check validity of configuration
  GridMatrix placement_matrix = generatePlacementMatrix(baglist, itemlist);

  std::cout << "Test";
  return EXIT_SUCCESS;
}

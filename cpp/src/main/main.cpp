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

int main(int argc, char** argv) {
  // TODO: Add argparser to choose strategy and parameters instead of hard-coding
  std::srand(std::time(nullptr)); // use current time as seed for random generator

  // Define item and bag inventory
  vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag"), LookupBag("StaminaSack"),};
  vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan"), LookupItem("GlovesOfHaste")};

  // Choose a search strategy
  // gridmatrix search_result = randomSearchStrategy(baglist, itemlist, 60, 11);
  gridmatrix search_result = greedySearchStrategy(baglist, itemlist, 60, 11);

  // If no valid configuration is found this will be all zeros
  printGridMatrix(search_result);

  return EXIT_SUCCESS;
}

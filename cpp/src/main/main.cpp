#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "items.h"
#include "bags.h"

int main(int argc, char** argv) {

  std::vector<Bag> baglist = {LookupBag("RangerBag"), LookupBag("RangerBag")};
  std::vector<Item> itemlist = {LookupItem("WoodenSword"), LookupItem("Pan")};

  std::cout << "Test";
  return EXIT_SUCCESS;
}
#include <cctype>
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
#include <algorithm>

#include "randomSearch.cpp"
#include "greedySearch.cpp"

using std::vector;

char* getCmdOption(char ** begin, char ** end, const std::string & option)
{
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

void printHelp() {
  std::cout << 
    "Usage: bb_solver [OPTIONS] -b BAGS -i ITEMS \n"
    "Example: bb_solver -b 12 -i 01 -a greedy -t 20 -n 11\n\n"
    "Example: bb_solver -a greedy -t 20 113 0233\n\n"
    "Options:\n"
    "  -b,  type and number of bags to place\n"
    "       format as a list of characters (e.g. 1224)\n"
    "       bag numbers:\n"
    "       0 - Fanny Pack\n"
    "       1 - Leather Bag\n"
    "       2 - Potion Belt\n"
    "       3 - Ranger Bag\n"
    "  -i,  type and number of items to place\n"
    "       format as a list of characters (e.g. 1224)\n"
    "       item numbers:\n"
    "       0 - Wooden Sword\n"
    "       1 - Pan\n"
    "       2 - Gloves of Haste\n"
    "  -a,  type of search algorithm (default \"greedy\")\n"
    "       Algorithms:\n" 
    "       \"random\", \"greedy\"\n"
    "  -t,  search time limit in seconds (default 10)\n"
    "  -n,  threads (default 1) \n";
}

int main(int argc, char **argv) {


  if (cmdOptionExists(argv, argv+argc, "-h") || 
    !(cmdOptionExists(argv, argv+argc, "-b") &&
    cmdOptionExists(argv, argv+argc, "-i"))) {
      printHelp();
      return 0;
  }

  //// Define item and bag inventory
  vector<Bag> baglist;
  vector<Item> itemlist;

  std::string bagstring(getCmdOption(argv, argv + argc, "-b"));
  std::string itemstring(getCmdOption(argv, argv + argc, "-i"));

  for (int i = 0; i < bagstring.length(); i++) {
    if (!std::isdigit(bagstring.at(i))) continue; 
    baglist.push_back(bag_table[(int) bagstring.at(i) - 48]);

    if (baglist.size() == 0) {
      printHelp(); return 0;
    }
  }

  for (int i = 0; i < itemstring.length(); i++) {
    if (!std::isdigit(itemstring.at(i))) continue; 
    itemlist.push_back(item_table[(int) itemstring.at(i) - 48]);

    if (itemlist.size() == 0) {
      printHelp(); return 0;
    }
  }

  std::srand(std::time(nullptr)); // use current time as seed for random generator
  std::string timelimit_string;
  std::string thread_string;
  std::string strategy_string;

  // Parse options 
  if (cmdOptionExists(argv, argv+argc, "-a")) {
    strategy_string = getCmdOption(argv, argv + argc, "-a");
  } else {
    strategy_string = "greedy";
  }

  if (cmdOptionExists(argv, argv+argc, "-t")) {
    timelimit_string = getCmdOption(argv, argv + argc, "-t");
  } else {
    timelimit_string = "10";
  }

  if (cmdOptionExists(argv, argv+argc, "-n")) {
    thread_string = getCmdOption(argv, argv + argc, "-n");
  } else {
    thread_string = "1";
  }

  // Run search
  gridmatrix search_result;
  std::cout << strategy_string << " is the string.\n";
  assert(strategy_string == "random" || strategy_string == "greedy");
  if (strategy_string == "random") {
    search_result = randomSearchStrategy(baglist, itemlist, std::stoi( timelimit_string), std::stoi(thread_string));
  } else if (strategy_string == "greedy") {
    search_result = greedySearchStrategy(baglist, itemlist, std::stoi( timelimit_string), std::stoi(thread_string));
  } 
  global_best_score = 0;

  // If no valid configuration is found this will be all zeros
  printGridMatrix(search_result);

  return EXIT_SUCCESS;
}

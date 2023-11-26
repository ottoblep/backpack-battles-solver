#include <vector>
#include <array>

class Bag {
    public:
        // 1x2 integer array = coordinates 
        std::array<int, 2> position;
        // 1=90degrees 2=180degrees etc
        int rotation;
        // vector of coordinates relative to origin
        std::vector<std::array<int, 2>> blocks;
        // Decides which other items can be modified by this bag
        int bag_type;

        Bag(std::vector<std::array<int, 2>> blocks)
          : blocks(blocks)
        {}
};
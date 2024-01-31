#include <vector>
#include <array>

class Item {
    public:
        // 1x2 integer array = coordinates 
        std::array<int, 2> position;
        // 1=90degrees 2=180degrees etc
        int rotation;
        // vector of coordinates relative to origin
        std::vector<std::array<int, 2>> blocks;
        // vector of coordinates that can connect the block 
        std::vector<std::array<int, 2>> connection_blocks;
        // Type(s) of item that can connect the block
        int connection_type;
        // Decides which other items can be modified by this item
        int item_type;

        Item(std::vector<std::array<int, 2>> blocks, std::vector<std::array<int, 2>> connection_blocks)
          : blocks(blocks)
          , connection_blocks(connection_blocks)
        {}
};
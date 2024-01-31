#include <string>
#include "item.cpp"

const Item item_table[] {
        Item({{0, 0},{1, 0}}, {}), // Wooden Sword
        Item({{0, 0},{1, 0},{1, 1}}, {{0, -1},{1, -1},{-1, 0}, {0, 1}, {1, 2}, {2, 0}, {2, 1}}), // Pan
        Item({{0, 0},{1, 0}}, {{}}) // Gloves of Haste
};
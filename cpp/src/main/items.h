#include <string>
#include "item.cpp"

Item LookupItem(std::string name) {
    if (name == "WoodenSword")
        return Item({{0, 0},{0, 1}}, {});
    if (name == "Pan")
        return Item({{0, 0},{1, 0},{1, 1}}, {{0, -1}, {1, -1}, {-1, 0}, {0, 1}, {1, 3}, {3, 0}, {3, 1}});
    if (name == "GlovesOfHaste")
        return Item({{0, 0},{1, 0}}, {{}});

    throw::std::invalid_argument("Item type not found.");
    return Item({},{});
};
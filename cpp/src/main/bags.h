#include <string>
#include "bag.cpp"

Bag LookupBag(std::string name) {
    if (name == "FannyPack")
        return Bag({{0, 0}});
    if (name == "StaminaSack")
        return Bag({{0, 0},{0, 1}, {0, 2}});
    if (name == "LeatherBag")
        return Bag({{0, 0},{1, 0}, {0, 1}, {1, 1}});
    if (name == "PotionBelt")
        return Bag({{0, 0},{0, 1}, {0, 2}, {0, 3}, {0, 4}});
    if (name == "RangerBag")
        return Bag({{0, 0},{0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}});

    return Bag({});
};
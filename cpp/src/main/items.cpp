#include "item.cpp"

class WoodenSword : Item {
    WoodenSword() : Item({{0, 0},{0, 1}}, {}) {}
};

class Pan : Item {
    Pan() : Item({{0, 0},{1, 0},{1, 1}},
                 {{0, -1}, {1, -1}, {-1, 0}, {0, 1}, {1, 3}, {3, 0}, {3, 1}}) {}
};

class GlovesOfHaste : Item {
    GlovesOfHaste() : Item({{0, 0},{1, 0}}, {{}}) {}
};
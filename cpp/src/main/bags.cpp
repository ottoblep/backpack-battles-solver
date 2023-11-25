#include "bag.cpp"

class ProtectivePurse : Bag {
    ProtectivePurse() : Bag({{0, 0}}) {}
};

class FannyPack : Bag {
    FannyPack() : Bag({{0, 0},{1, 0}}) {}
};

class StaminaSack : Bag {
    StaminaSack() : Bag({{0, 0},{0, 1}, {0, 2}}) {}
};

class LeatherBag : Bag {
    LeatherBag() : Bag({{0, 0},{1, 0}, {0, 1}, {1, 1}}) {}
};

class PotionBelt : Bag {
    PotionBelt() : Bag({{0, 0},{0, 1}, {0, 2}, {0, 3}, {0, 4}}) {}
};

class RangerBag : Bag {
    RangerBag() : Bag({{0, 0},{0, 1}, {1, 0}, {1, 1}, {2, 0}, {2, 1}}) {}
};
# Backpack Battles Solver

This repo aims to solve the placement/optimization problem posed in the game [Backpack Battles](https://playwithfurcifer.itch.io/backpack-battles).

```
Usage: bb_solver [OPTIONS] -b BAGS -i ITEMS 
Example: bb_solver -b 12 -i 01 -a greedy -t 20 -n 11

Options:
  -b,  type and number of bags to place
       format as a list of characters (e.g. 1224)
       bag numbers:
       0 - Fanny Pack
       1 - Leather Bag
       2 - Potion Belt
       3 - Ranger Bag
  -i,  type and number of items to place
       format as a list of characters (e.g. 1224)
       item numbers:
       0 - Wooden Sword
       1 - Pan
       2 - Gloves of Haste
  -a,  type of search algorithm (default "greedy")
       Algorithms:
       "random", "greedy"
  -t,  search time limit in seconds (default 10)
  -n,  threads (default 1) 
```
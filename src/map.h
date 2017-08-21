#ifndef MAP_H
#define MAP_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

struct Map
{
    bool validMap = false;
    std::vector<int> safeRooms = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
    std::vector<std::vector<int>> initializeMap();
};
#endif

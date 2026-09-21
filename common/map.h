#ifndef MAP_H
#define MAP_H

#include "io/filing.h"
#include "components/tile.h"

const char* path = "../resources/map.bin";

class Map
{
private:
    std::vector<Tile> bin_map;

    void loadMap(const char* path)
    {
        filing::loadFromFile(path, bin_map);
        std::stable_sort(bin_map.begin(), bin_map.end());
    }

public:
    Map();
    ~Map();

    void init()
    {
        loadMap(path);
    }
};
#endif // MAP_H
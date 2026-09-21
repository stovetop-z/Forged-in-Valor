#ifndef TILE_H
#define TILE_H

#include <cstdint>

struct Tile
{
    uint8_t layer, size, physics, id;
    uint16_t x, y;

    float scale = 3.0f;
    bool operator<(const Tile& b) const
    {
        return b.layer > layer;
    }
};  
#endif
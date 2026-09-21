#ifndef FILING_H
#define FILING_H

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string_view>
#include <vector>

namespace filing
{
    template<typename T>
    bool loadFromFile(const char* path, std::vector<T>& map)
    {
        std::ifstream f(path, std::ios::binary);
        if (!f.is_open())
        {
            std::fprintf(stderr, "Can't open file %s, check path and filename.\n", path);
            return false;
        }

        uint8_t layer = 0;
        uint8_t size = 0;
        uint8_t id = 0;
        uint8_t physics = 0;
        uint16_t x = 0;
        uint16_t y = 0;

        // Loop checks whether every field was read successfully before pushing
        while (f.read(reinterpret_cast<char*>(&id), sizeof(id)) &&
               f.read(reinterpret_cast<char*>(&layer), sizeof(layer)) &&
               f.read(reinterpret_cast<char*>(&size), sizeof(size)) &&
               f.read(reinterpret_cast<char*>(&x), sizeof(x)) &&
               f.read(reinterpret_cast<char*>(&y), sizeof(y)) && 
               f.read(reinterpret_cast<char*>(&physics), sizeof(physics)))
        {
            T t{
                .layer = layer,
                .size = size,
                .x = x,
                .y = y,
                .id = id,
                .physics = physics
            };

            map.push_back(t);
        }

        return true;
    }

    template<typename T>
    bool saveToFile(const char* path, const std::vector<T>& map, bool append = false)
    {
        auto mode = std::ios::binary | (append ? std::ios::app : std::ios::trunc);
        std::ofstream f(path, mode);

        if (!f.is_open())
        {
            std::fprintf(stderr, "Can't open file %s, check path and filename.\n", path);
            return false;
        }

        for (const auto& bin : map)
        {
            f.write(reinterpret_cast<const char*>(&bin.id), sizeof(bin.id));
            f.write(reinterpret_cast<const char*>(&bin.layer), sizeof(bin.layer));
            f.write(reinterpret_cast<const char*>(&bin.size), sizeof(bin.size));
            f.write(reinterpret_cast<const char*>(&bin.x), sizeof(bin.x));
            f.write(reinterpret_cast<const char*>(&bin.y), sizeof(bin.y));
            f.write(reinterpret_cast<const char*>(&bin.physics), sizeof(bin.physics));
        }

        return f.good();
    }
}

#endif // FILING_H
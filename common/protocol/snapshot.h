#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <cstdint>
#include <vector>
#include "../math/vector2.h"

struct State
{
    uint8_t skin_id, anim_state, health;
    uint32_t id;
    V2 position;
};

struct Snapshot
{
    uint32_t server_tick;
    std::vector<State> states;
};
#endif
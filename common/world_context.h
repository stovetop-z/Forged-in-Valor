#ifndef WORLD_CONTEXT_H
#define WORLD_CONTEXT_H

#include <vector>
#include <memory>

class Entity;

struct WorldContext
{
    const std::vector<std::unique_ptr<Entity>>& entities;

    const Entity* local_player = nullptr;
};
#endif